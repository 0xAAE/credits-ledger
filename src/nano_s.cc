#include "nano_s.h"

#include <cscrypto/cscrypto.hpp>
#include <iostream>

#include "packet.h"

namespace ledger {

NanoS::NanoS() {
  hid_api_.registerApiErrorCallback([](HidError err) {
    std::cout << "HidApiErr: " << err.getErrorString() << std::endl;
  });

  hid_api_.registerDeviceErrorCallback([](HidDevice dev, HidError err) {
    std::cout << "HidApiDeviceErr: " << err.getErrorString()
              << " - " << dev.getPath() << std::endl;
  });
}

void NanoS::Open() {
  hid_devices_ = hid_api_.scanDevices(kLedgerVendId,
                                      ANY, ANY, ANY, ANY, ANY, ANY, ANY);

  if (hid_devices_.size() != 1) {
    throw std::runtime_error("Wrong num of ledger devices connected.");
  }

  nanos_ = hid_devices_[0];

  if (!nanos_.isInitialized()) {
    throw std::runtime_error("Cannot initialize ledger device.");
  }

  if (!nanos_.open()) {
    throw std::runtime_error("Cannot open ledger device.");
  }
}

cscrypto::PublicKey NanoS::GetPublicKey(uint32_t index) {
  CheckOpen();

  auto packs = Packet::FormPackets(reinterpret_cast<uint8_t*>(&index),
                                   sizeof(index),
                                   Packet::Instruction::kGetPubliKey);
  SendPackets(std::move(packs));

  auto bytes = Packet::TakeTargetBytes({nanos_.read(kBlockingRead)},
                                       Packet::Instruction::kGetPubliKey);
  cscrypto::PublicKey result;
  std::copy(bytes.begin(), bytes.end(), result.begin());
  return result;
}

cscrypto::Signature NanoS::Sign(uint32_t key_index, const cscrypto::Hash& hash) {
  CheckOpen();

  std::vector<uint8_t> data(sizeof(key_index) + hash.size());
  auto ptr = reinterpret_cast<uint8_t*>(&key_index);
  std::copy(ptr, ptr + sizeof(key_index), data.begin());
  std::copy(hash.begin(), hash.end(), data.begin() + sizeof(key_index));

  auto packs = Packet::FormPackets(data.data(), data.size(), Packet::Instruction::kSignHash);
  SendPackets(std::move(packs));

  std::vector<std::string> signature_packs;
  signature_packs.push_back(nanos_.read(kBlockingRead));
  signature_packs.push_back(nanos_.read(kBlockingRead));

  auto bytes = Packet::TakeTargetBytes(signature_packs, Packet::Instruction::kSignHash);

  cscrypto::Signature res;
  std::copy(bytes.begin(), bytes.end(), res.begin());
  return res;
}

void NanoS::CheckOpen() {
  if (!nanos_.open()) {
    throw std::runtime_error("Cannot open ledger device.");
  }
}

void NanoS::SendPackets(std::vector<std::string>&& packets) {
  for (auto& p : packets) {
    if (!nanos_.write(p)) {
      throw std::runtime_error("Cannot write to Nano S.");
    }
  }
}

} // namespace ledger
