#import <Cocoa/Cocoa.h>

@interface XYZContentView : NSView
{
  NSTabView * tabView_;
}
- (NSTabView *)tabView;
- (void)addNewTab:(id)sender;
- (unsigned int)tabHeight;
@end // XYZContentView

@interface XYZTextView : NSView
{
  NSColor *bgColor_;
  NSString *string_;
}
@end // XYZTextView
