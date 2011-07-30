#import "XYZWindow.h"
#import "XYZApplication.h"

int main(int argc, char *argv[])
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSApplication *app = [NSApplication sharedApplication];
  XYZApplication *xyz = [[XYZApplication alloc] init];
  [app setDelegate:xyz];
  [app run];

  [pool drain];
  [xyz release];

  return 0;
}
