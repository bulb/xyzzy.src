#import "XYZApplication.h"
#import "XYZWindow.h"
#import "osx_if.h"

@implementation XYZApplication
- (id)init
{
  if ((self = [super init])) {
    
    XYZWindow *win = [[XYZWindow alloc] init];
    [win setupWindow];
    main1(); // lisp init
    [win setupAppMenu];
  }

  return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)aNotification
{
  NSLog(@"DidFinish");
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
  NSLog(@"WillTerminate");
}
@end // XYZApplication
