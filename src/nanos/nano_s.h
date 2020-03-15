#ifndef NANO_S_H
#define NANO_S_H

#include <cscrypto/cscrypto.hpp>
#include <HidApi.h>
#include "packet.h"

#include <stdexcept>

namespace ledger {

class NanoS {
 public:
  NanoS();

  void Open();
  cscrypto::PublicKey GetPublicKey(uint32_t index);
  cscrypto::Signature Sign(uint32_t key_index, const cscrypto::Hash& hash);

 private:
  void CheckOpen();
  void SendPackets(std::vector<std::string>&&);
  std::vector<std::string> ReadPackets(Packet::Instruction);

  constexpr static uint16_t kLedgerVendId = 0x2c97;
  constexpr static int kBlockingRead = -1;

  constexpr static uint8_t kNumPacksGetPublicKey = 1;
  constexpr static uint8_t kNumPacksSignHash = 2;

  HidApi hid_api_;
  HidDeviceList hid_devices_;
  HidDevice nanos_;
};
} // namespace ledger
#endif // NANO_S_H
