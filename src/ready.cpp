#include "ready.h"
bool SIMReady=0;

bool getSIMReady(void)
{
  return SIMReady;
}
void setSIMReady(void)
{
  SIMReady = 1;
}
void setnotSIMReady(void)
{
  SIMReady = 0;
}
