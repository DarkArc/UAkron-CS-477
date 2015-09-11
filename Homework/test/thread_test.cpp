#include "../../include/thread.h"


int main()
{
  join(
    create_thread(
      [&] {
        printf("Thread created");
      }
    )
  );

  return 0;
}
