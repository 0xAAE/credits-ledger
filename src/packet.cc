#include "packet.h"

#include <algorithm>

#include <cscrypto/cscrypto.hpp>

namespace ledger {

std::vector<std::string> Packet::FormPackets(uint8_t* data,
                                             uint8_t len,
                                             Instruction ins,
                                             uint8_t p1,
                                             uint8_t p2) {
  std::vector<std::string> result;
  uint8_t sequence = 0;
  result.emplace_back(Packet(data, len, ins, p1, p2, sequence).TakeData());
  int16_t remaining_len = len - (kHidPackSize - kFirstHidHeaderSize - kApduHeaderSize);

  while (remaining_len > 0) {
    Packet pack(data + len - remaining_len, remaining_len, ins, p1, p2, ++sequence);
    result.emplace_back(pack.TakeData());
    remaining_len -= (kHidPackSize - kCommonHidHeaderSize - kApduHeaderSize);
  }

  return result;
}

Packet::Packet(uint8_t* data, uint8_t len, Instruction ins, uint8_t p1, uint8_t p2, uint8_t sq) {
  data_.resize(kHidPackSize, 0);

  data_[kChannelH] = kChannelId;
  data_[kChannelL] = kChannelId;
  data_[kTag] = kCommandTag;
  data_[kPacketSeqL] = sq;

  uint8_t offset = kCommonHidHeaderSize;

  if (sq == 0) {
    uint16_t payload_len = len + kApduHeaderSize;
    data_[kPayloadLenH] = (payload_len >> 8);
    data_[kPayloadLenL] = (payload_len & 0x00FF);
    offset = kFirstHidHeaderSize;
  }

  data_[offset + kCla] = kClaVal;
  data_[offset + kIns] = static_cast<char>(ins);
  data_[offset + kP1] = p1;
  data_[offset + kP2] = p2;
  data_[offset + kLen] = len;

  char* ptr = reinterpret_cast<char*>(data);
  std::copy(ptr,
            ptr + std::min(int(len), kHidPackSize - offset - kApduHeaderSize),
            data_.data() + offset + ApduOffsets::kData);
}

bool Packet::CheckHeader(const std::string& data, uint8_t sequence) {
  return
    data.size() == kHidPackSize &&
    data[kChannelH] == kChannelId &&
    data[kChannelL] == kChannelId &&
    data[kTag] == kCommandTag &&
    data[kPacketSeqL] == sequence;
}

uint16_t Packet::GetTotalLen(const std::string& data) {
  uint16_t res;
  uint8_t* p = reinterpret_cast<uint8_t*>(&res);
  p[0] = data[kPayloadLenL];
  p[1] = data[kPayloadLenH];
  return res;
}

std::vector<uint8_t> Packet::TakeTargetBytes(const std::vector<std::string>& packets,
                                             Instruction ins) {
  std::vector<uint8_t> res;
  uint8_t current_seq = 0;
  int32_t total_len = 0;

  for (auto& p : packets) {
    if (!CheckHeader(p, current_seq)) throw std::runtime_error("Invalid Packet header.");

    if (current_seq == 0) {
      total_len = GetTotalLen(p);
      bool ok = (ins == Instruction::kGetPubliKey &&
                 total_len == cscrypto::kPublicKeySize + sizeof(ErrorCode)) ||
                (ins == Instruction::kSignHash &&
                 total_len == cscrypto::kSignatureSize + sizeof(ErrorCode));
      if (!ok) throw std::runtime_error("Invalid Packet Len.");
    }

    auto offset = current_seq == 0 ? kFirstHidHeaderSize : kCommonHidHeaderSize;
    res.insert(res.end(),
               p.begin() + offset,
               total_len > (kHidPackSize - offset) ?
                           p.end() : p.begin() + offset + total_len - sizeof(ErrorCode));
    ++current_seq;
    total_len = total_len - (kHidPackSize - offset);
  }

  return res;
}

} // namespace ledger
