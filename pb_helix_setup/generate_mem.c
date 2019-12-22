#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Si5395-RevA-HXMTRIG-160M-Registers.h"

int main() {
  FILE *f = fopen("pb_helix_setup.mem", "w+");
  fprintf(f, "@00\n");
  // if this is 1, then we write 0x4, because that's the final address.
  fprintf(f, "%8.8x\n", SI5395_REVA_REG_CONFIG_NUM_REGS*4);
  for (int i=0;i<SI5395_REVA_REG_CONFIG_NUM_REGS;i++) {
    uint32_t val;
    val = si5395_reva_registers[i].value;
    val = val + (si5395_reva_registers[i].address << 8);
    fprintf(f, "%8.8x\n", val);
  }
  for (int i=SI5395_REVA_REG_CONFIG_NUM_REGS;i<1024;i++) {
    uint32_t val;
    val = 0;
    fprintf(f, "%8.8x\n", val);
  }
  fflush(f);
  fclose(f);
}
