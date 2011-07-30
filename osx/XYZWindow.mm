#import "XYZWindow.h"
#import "XYZView.h"

@implementation XYZWindow
- (id)init
{
  
  if (self = [super init]) {
  }

  return self;
}

- (void)setupAppMenu
{
  NSMenu *appleMenu = [[NSMenu alloc] initWithTitle:@""];
  NSMenuItem *menuItem;
  NSString *title;
  NSString *appName = @"xyzzy";


  [NSApp setMainMenu:[[NSMenu alloc] init]];


  // About
  title = [@"About " stringByAppendingString:appName];
  [appleMenu addItemWithTitle:title
	     action:@selector(orderFrontStandbountPanel:)
	     keyEquivalent:@""];
  // ----
  [appleMenu addItem:[NSMenuItem separatorItem]];

  // Hide
  [appleMenu addItemWithTitle:[@"Hide " stringByAppendingString:appName]
	     action:@selector(hide:)
	     keyEquivalent:@"h"];

  // Hide others
  menuItem = [appleMenu addItemWithTitle:@"Hide Others"
			action:@selector(hideOtherApplications:)
			keyEquivalent:@"h"];
  [menuItem setKeyEquivalentModifierMask:(NSAlternateKeyMask |
					  NSCommandKeyMask)];
  // Show All
  [appleMenu addItemWithTitle:@"Show all"
	     action:@selector(unhideAllApplications:)
	     keyEquivalent:@""];
  // ----
  [appleMenu addItem:[NSMenuItem separatorItem]];

  title = [@"Quit " stringByAppendingString:appName];
  [appleMenu addItemWithTitle:title
	     action:@selector(terminate:)
	     keyEquivalent:@"q"];

  menuItem = [[NSMenuItem alloc] initWithTitle:@""
				 action:nil
				 keyEquivalent:@""];

  [menuItem setSubmenu:appleMenu];
  [[NSApp mainMenu] addItem:menuItem];
  [NSApp performSelector:@selector(setAppleMenu:) withObject:appleMenu];
 
  [menuItem release];
  [appleMenu release];
}

- (id)initWithContentRect:(NSRect)rect
{
  NSUInteger winStyle = (NSTitledWindowMask
			 | NSClosableWindowMask
			 | NSMiniaturizableWindowMask
			 | NSResizableWindowMask
			 | NSTexturedBackgroundWindowMask
			 );

  if ([super initWithContentRect:rect
	               styleMask:winStyle
	                 backing:NSBackingStoreBuffered
	                   defer:YES])
    {
      [self setViewsNeedDisplay:NO];
      [self setTitle:@"xyzzy"];
      [self setOpaque:YES];

      // tab
      contentView_ = [[XYZContentView alloc] initWithFrame:rect];
      [self setContentView:contentView_];
      [self orderFront: self];

      // text
      NSRect r = NSMakeRect(0, [contentView_ tabHeight],
			    rect.size.height, rect.size.width);
      textView_ = [[XYZTextView alloc] initWithFrame:r];
      [[self contentView] addSubview:textView_];
    }


  return self;
}

- (void)setupWindow
{
  NSRect rect = NSMakeRect(200, 200, 400, 400);

  window_ = [[XYZWindow alloc] initWithContentRect:rect];
}

- (XYZTextView *)textView
{
  return textView_;
}

- (void)setTextView:(XYZTextView *)view
{
  if (textView_)
    [textView_ removeFromSuperview];
  textView_ = view;
  [[self contentView] addSubview:textView_];
}

- (XYZContentView *)contentView
{
  return contentView_;
}

- (void)delloc
{
  NSLog(@"window dealloc");
  [super dealloc];
}

//
// Delegate Methods
//
- (void)windowDidBecomeKey:(NSNotification *)notification
{
  NSLog(@"Did BecomeKey ...");
}

- (void)windowDidBecomeMain:(NSNotification *)notification
{
  NSLog(@"Did BecomeMain ...");
}

-(void)windowDidChangeScreen:(NSNotification *)notofication
{
  NSLog(@"windowDidChangeScreen");
}

- (void)windowDidEndSheet:(NSNotification *)notification
{
  NSLog(@"Did EndSheet ...");
}

- (void)windowDidExpose:(NSNotification *)notification
{
  NSLog(@"Did Expose ...");
}

- (void)windowDidResize:(NSNotification *)notification
{
  NSLog(@"Did Resize ...");
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
  NSLog(@"Did Demini ...");
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
  NSLog(@"Did Miniature ...");
}

- (void)windowWillMiniaturize:(NSNotification *)notification
{
  NSLog(@"Will Miniature ...");
}

- (void)windowDidMove:(NSNotification *)notification
{
  NSLog(@"Did Move ...");
}

- (void)windowWillMove:(NSNotification *)notification
{
  NSLog(@"Will Move ...");
}

- (void)windowDidUpdate:(NSNotification *)notification
{
  NSLog(@"Did Update ...");
}

- (BOOL)windowShouldClose:(NSNotification *)notification
{
  NSLog(@"ShouldClose ...");
  return YES;
}

- (void)windowWillClose:(NSNotification *)notification
{
  NSLog(@"WillClose ...");
}

@end // XYZWindow
