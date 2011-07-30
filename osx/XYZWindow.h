#import <Cocoa/Cocoa.h>
#import "XYZView.h"

@interface XYZWindow : NSWindow
{
  XYZWindow * window_;
  XYZTextView * textView_;
  XYZContentView * contentView_;
}
- (void)setupWindow;
- (void)setupAppMenu;
@end // XYZWindow
