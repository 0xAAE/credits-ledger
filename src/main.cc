#include <iostream>

#include <cscrypto/cscrypto.hpp>
#include "nano_s.h"

int main() {
  ledger::NanoS nanos;

  try {
    nanos.Open();
    auto public_key = nanos.GetPublicKey(13);
    std::cout << "Received public key:" << std::endl;
    std::cout << "HEX: " << cscrypto::helpers::bin2Hex(public_key.data(), public_key.size()) << std::endl;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
