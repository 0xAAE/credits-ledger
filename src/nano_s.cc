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

  is_open_ = true;
}

cscrypto::PublicKey NanoS::GetPublicKey(uint32_t index) {
  if (!is_open_) {
    throw std::domain_error("Nano S is not open.");
  }

  auto packs = Packet::FormPackets(reinterpret_cast<uint8_t*>(&index),
                                   sizeof(index),
                                   Packet::Instruction::kGetPubliKey);
  for (auto& p : packs) {
    if (!nanos_.write(p)) {
      throw std::runtime_error("Cannot write to Nano S.");
    }
  }

  auto bytes = Packet::TakeTargetBytes({nanos_.read(kBlockingRead)},
                                       Packet::Instruction::kGetPubliKey);
  cscrypto::PublicKey result;
  std::copy(bytes.begin(), bytes.end(), result.begin());
  return result;
}

} // namespace ledger
