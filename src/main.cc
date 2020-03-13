#include <chrono>
#include <iostream>
#include <thread>

#include <cscrypto/cscrypto.hpp>
#include "nano_s.h"

int main() {
  ledger::NanoS nanos;
  const uint32_t key_index = 13;
  const std::vector<uint8_t> data{1,2,3};

  try {
    nanos.Open();

    auto public_key = nanos.GetPublicKey(key_index);

    std::cout << "Received public key:" << std::endl;
    std::cout << "HEX: " << cscrypto::helpers::bin2Hex(public_key.data(), public_key.size()) << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds{5});

    auto hash = cscrypto::calculateHash(data.data(), data.size());

    std::cout << "Hash to sign: " << cscrypto::helpers::bin2Hex(hash.data(), hash.size()) << std::endl
              << "Key index: " << key_index << std::endl;

    auto signature = nanos.Sign(key_index, hash);

    bool res = cscrypto::verifySignature(signature, public_key, hash.data(), hash.size());
    std::cout << "Verify signature result: " << res << std::endl;

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
