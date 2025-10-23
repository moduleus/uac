#include <ios>
#include <iostream>
#include <ostream>
#include <string>

#include <urx/version.h>

#include <uac/version.h>

int main(int argc, char** argv) {
  const uac::Version uac_version;
  const urx::Version urx_version;
  std::cout << "UAC v" << uac_version.major << "." << uac_version.minor << "." << uac_version.patch
            << "\n";

  std::cout << "URX v" << urx_version.major << "." << urx_version.minor << "." << urx_version.patch
            << "\n";

  return 0;
}
