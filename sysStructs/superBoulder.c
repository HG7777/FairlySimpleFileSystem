#include "superBoulder.h"

#include "../sizes.h"
#include "../funSizeSys/readWriteSys.h"

struct Superboulder getSuperBoulder() {
  struct Superboulder superboulder;
  readSys(SB_OFFSET, (void*) &superboulder, sizeof(struct Superboulder));

  return superboulder;
}
