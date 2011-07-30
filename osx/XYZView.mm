#import "XYZView.h"
#import "osx_if.h"

@implementation XYZTextView

- (id)init
{
  self = [super init];
  if (self) {
  }

  return self;
}

- (id)initWithFrame:(NSRect)frame
{
  self = [super initWithFrame:frame];
  if (self) {
    [[self window] makeFirstResponder:self];
    bgColor_ = [[NSColor whiteColor] retain];
  }

  return self;
}

- (void)drawRect:(NSRect)rect
{
	NSRect bounds = [self bounds];
	[bgColor_ set];
	[NSBezierPath fillRect:bounds];
	
	if ([[self window] firstResponder] == self) {
		[[NSColor keyboardFocusIndicatorColor] set];
		[NSBezierPath setDefaultLineWidth:5.0];
		[NSBezierPath strokeRect:bounds];
	}
	
	// Initialize.
	CGContextRef ctxt = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
	CGContextSetTextMatrix(ctxt, CGAffineTransformIdentity);
	
	// an attributed string.
	CFStringRef str;

	unsigned int us_used = stringLength();
	if (us_used != 0 ) {
		str = CFStringCreateWithCharacters(kCFAllocatorDefault, stringText(), us_used);
	}
	else {
		return;
	}

	CFMutableAttributedStringRef attrStr
	= CFAttributedStringCreateMutable(kCFAllocatorDefault, 0);
	CFAttributedStringReplaceString(attrStr, 
									CFRangeMake(0, 0),
									str);
	
	CGMutablePathRef path = CGPathCreateMutable();
	CGPathAddRect(path, NULL, NSRectToCGRect(rect));
	
	CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(attrStr);
	CFRelease(attrStr);
	
	CTFrameRef frame
	= CTFramesetterCreateFrame(framesetter, 
							   CFRangeMake(0, 0), 
							   path, 
							   NULL);
	
	CTFrameDraw(frame, ctxt);
	
	CFRelease(frame);
	CFRelease(framesetter);
	CFRelease(path);
	
}

- (void)setString:(NSString *)c
{
  c = [c copy];
  [string_ release];
  string_ = c;

  const char *str = [c UTF8String];
  int ch = str[0];

  dummy_loop(ch);
}

- (void)insertText:(NSString *)input
{
  [self setString:input];
  [self setNeedsDisplay:YES];
}

- (void)insertNewline:(id)sender
{
  [self insertText:@"\n"];
}

- (void)insertTab:(id)sender
{
  [self insertText:@"\t"];
}

- (void)keyDown:(NSEvent *)event
{
  NSLog(@"keyDown");
  [self interpretKeyEvents:[NSArray arrayWithObject:event]];
}

- (void)dealloc
{
  NSLog(@"view dealloc");
  [super dealloc];
}

- (BOOL)acceptsFirstResponder
{
  return YES;
}

- (BOOL)acceptsFirstMouse:(NSEvent *)event
{
  NSLog(@"acceptsFisrtMouse");
  return YES;
}

- (BOOL)resignFirstResponder
{
  NSLog(@"resignFirstResponder");
  [self setNeedsDisplay:YES];
  return YES;
}

- (BOOL)becomeFirstResponder
{
  NSLog(@"becomeFirstResponder");
  [self setNeedsDisplay:YES];
  return YES;
}

- (id)menuDown:sender
{
  return self;
}

- (void)mouseDown:(NSEvent *)theEvent
{
  NSLog(@"mouseDown ...");
}

- (void)mouseUp:(NSEvent *)theEvent
{
  NSLog(@"mouseUp ...");
}

- (BOOL) isFlipped
{
  return NO;
}

@end // XYZTextView


@implementation XYZContentView

- (unsigned int)tabHeight
{
  return 0; // 20
}

- (BOOL)isFlipped
{
  return YES;
}

- (NSTabView *)tabView
{
  return tabView_;
}

- (void)addNewTab:(id)sender
{

}

- (void)drawRect:(NSRect)rect
{
  [[NSColor grayColor] set];
  NSRectFill(rect);
}

@end // XYZContentView

