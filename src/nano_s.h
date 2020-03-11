#ifndef NANO_S_H
#define NANO_S_H

#include <cscrypto/cscrypto.hpp>
#include <HidApi.h>

#include <stdexcept>

namespace ledger {

class NanoS {
 public:
  NanoS();

  void Open();
  cscrypto::PublicKey GetPublicKey(uint32_t index);
  cscrypto::Signature Sign(const std::vector<uint8_t>& data);

 private:
  constexpr static uint16_t kLedgerVendId = 0x2c97;

  enum class LedgerProtocol : uint8_t {
    kChannelId = 0x01,
    kCommandTag = 0x05,
    kCla = 0xE0
  };

  enum class Offsets : uint8_t {
    kChannel = 0,
    kTag = 2,
    kPacketSeq = 3,
    kApduLength = 5,
    kApduCla = 7,
    kApduIns = 8,
    kApduP1 = 9,
    kApduP2 = 10,
    kApduDataLength = 11,
    kApduData = 12
  };

  HidApi hid_api_;
  HidDeviceList hid_devices_;
  HidDevice nanos_;

  std::array<uint8_t, 64> hid_data_;
  bool is_open_ = false;
};
} // namespace ledger
#endif // NANO_S_H
