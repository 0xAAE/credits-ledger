#ifndef PACKET_H
#define PACKET_H

#include <array>
#include <cinttypes>
#include <string>
#include <vector>

namespace ledger {

class Packet {
 public:
  enum class Instruction : uint8_t {
    kVersion = 0x01,
    kSignHash = 0x02,
    kGetPubliKey = 0x03
  };

  static std::vector<std::string> FormPackets(uint8_t* data,
                                              uint8_t len,
                                              Instruction ins,
                                              uint8_t p1 = 0,
                                              uint8_t p2 = 0);

  static std::vector<uint8_t> TakeTargetBytes(const std::vector<std::string>& packets,
                                              Instruction ins);

 private:
  Packet(uint8_t* data, uint8_t len, Instruction ins, uint8_t p1, uint8_t p2, uint8_t sq);

  std::string&& TakeData() { return std::move(data_); }
  static bool CheckHeader(const std::string&, uint8_t sq);
  static uint16_t GetTotalLen(const std::string&);

  constexpr static uint8_t kHidPackSize = 64;
  constexpr static uint8_t kFirstHidHeaderSize = 7;
  constexpr static uint8_t kCommonHidHeaderSize = 5;
  constexpr static uint8_t kApduHeaderSize = 5;

  constexpr static uint8_t kClaVal = 0xE0;

  enum class LedgerProtocol : uint8_t {
    kChannelId = 0x01,
    kCommandTag = 0x05
  };

  enum HeaderOffsets : uint8_t {
    kChannelH = 0,
    kChannelL = 1,
    kTag = 2,
    kPacketSeqH = 3,
    kPacketSeqL = 4,
    kPayloadLenH = 5,
    kPayloadLenL = 6,
  };

  enum ApduOffsets : uint8_t {
    kCla = 0,
    kIns = 1,
    kP1 = 2,
    kP2 = 3,
    kLen = 4,
    kData = 5
  };

  enum ErrorCode : uint16_t {
    kOk = 0x9000
  };

  std::string data_;
};

} // namespace ledger
#endif // PACKET_H
