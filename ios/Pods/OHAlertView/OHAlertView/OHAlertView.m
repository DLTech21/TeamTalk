//
//  OHAlertView.m
//  AliSoftware
//
//  Created by Olivier on 30/12/10.
//  Copyright 2010 AliSoftware. All rights reserved.
//

#import "OHAlertView.h"
#import <objc/runtime.h>

#if __IPHONE_OS_VERSION_MIN_REQUIRED < 80000
#define USE_UIALERTVIEW 1
#else
#define USE_UIALERTVIEW 0
#endif


#if USE_UIALERTVIEW
@interface OHAlertView () <UIAlertViewDelegate>
@property(nonatomic, weak) UIAlertView* alert;
#else
@interface OHAlertView () <UITextFieldDelegate>
@property(nonatomic, weak) UIAlertController* alert;
#endif

@property (nonatomic, copy) NSString* title;
@property (nonatomic, copy) NSString* message;
@property (nonatomic, copy) NSString* cancelButtonTitle;
@property (nonatomic, copy) NSArray* otherButtonTitles;
@property (nonatomic, copy) OHAlertViewButtonHandler buttonHandler;

@end



@implementation OHAlertView
@synthesize buttonHandler = _buttonHandler;

/////////////////////////////////////////////////////////////////////////////
#pragma mark - Commodity Constructors

+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
             cancelButton:(NSString *)cancelButtonTitle
             otherButtons:(NSArray *)otherButtonTitles
            buttonHandler:(OHAlertViewButtonHandler)handler
{
	OHAlertView* alert = [[self alloc] initWithTitle:title
                                             message:message
                                        cancelButton:cancelButtonTitle
                                        otherButtons:otherButtonTitles
                                       buttonHandler:handler];
	[alert show];
}

+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
               alertStyle:(OHAlertViewStyle)alertStyle
             cancelButton:(NSString *)cancelButtonTitle
             otherButtons:(NSArray *)otherButtonTitles
            buttonHandler:(OHAlertViewButtonHandler)handler
{
	OHAlertView* alert = [[self alloc] initWithTitle:title
                                             message:message
                                        cancelButton:cancelButtonTitle
                                        otherButtons:otherButtonTitles
                                       buttonHandler:handler];
    alert.alertViewStyle = alertStyle;
	[alert show];
}

+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
            dismissButton:(NSString *)dismissButtonTitle
{
    [self showAlertWithTitle:title
                     message:message
                cancelButton:dismissButtonTitle
                otherButtons:nil
               buttonHandler:nil];
}

#pragma mark - Instance Methods

-(instancetype)initWithTitle:(NSString *)title
                     message:(NSString *)message
                cancelButton:(NSString *)cancelButtonTitle
                otherButtons:(NSArray *)otherButtonTitles
               buttonHandler:(OHAlertViewButtonHandler)handler
{
    self = [super init];
    if (self)
    {
        self.title = title;
        self.message = message;
        self.cancelButtonTitle = cancelButtonTitle;
        self.otherButtonTitles = otherButtonTitles;
        self.buttonHandler = handler;

        _cancelButtonIndex = cancelButtonTitle ? 0 : -1;
        _firstOtherButtonIndex = otherButtonTitles ? _cancelButtonIndex+1 : -1;
    }
	return self;
}

- (void)dealloc
{
    #if USE_UIALERTVIEW == 0
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UITextFieldTextDidChangeNotification
                                                  object:nil];
    #endif
}

-(void)show
{
#if USE_UIALERTVIEW
    
    // Note: need to send at least the first button because if the otherButtonTitles parameter is nil, self.firstOtherButtonIndex will be -1
    NSString* firstOther = (self.otherButtonTitles && ([self.otherButtonTitles count]>0)) ? [self.otherButtonTitles objectAtIndex:0] : nil;
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:self.title
                                                    message:self.message
                                                   delegate:self
                                          cancelButtonTitle:self.cancelButtonTitle
                                          otherButtonTitles:firstOther,nil];
    for (NSInteger idx=1; idx<self.otherButtonTitles.count; ++idx) {
        [alert addButtonWithTitle:self.otherButtonTitles[idx]];
    }
    switch (self.alertViewStyle)
    {
        case OHAlertViewStyleDefault:
            alert.alertViewStyle = UIAlertViewStyleDefault;
            break;
        case OHAlertViewStylePlainTextInput:
            alert.alertViewStyle = UIAlertViewStylePlainTextInput;
            break;
        case OHAlertViewStyleSecureTextInput:
            alert.alertViewStyle = UIAlertViewStyleSecureTextInput;
            break;
        case OHAlertViewStyleLoginAndPasswordInput:
            alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
            break;
        case OHAlertViewStyleEmailAndPasswordInput:
            alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
            [alert textFieldAtIndex:0].keyboardType = UIKeyboardTypeEmailAddress;
            break;
    }
    _cancelButtonIndex = alert.cancelButtonIndex;
    _firstOtherButtonIndex = alert.firstOtherButtonIndex;
    self.alert = alert;
    [self makeAlertRetainSelf];
    [alert show];
    
#else
    
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:self.title
                                                                   message:self.message
                                                            preferredStyle:UIAlertControllerStyleAlert];
    void (^(^handler)(NSInteger))(UIAlertAction*) = ^(NSInteger index){
        return ^(UIAlertAction* _) {
            if (self.buttonHandler) {
                self.buttonHandler(self, index);
            }
        };
    };
    
    if (self.cancelButtonTitle)
    {
        [alert addAction:[UIAlertAction actionWithTitle:self.cancelButtonTitle style:UIAlertActionStyleCancel handler:handler(self.cancelButtonIndex)]];
    }
    
    NSInteger idx = self.firstOtherButtonIndex;
    for (NSString* buttonTitle in self.otherButtonTitles)
    {
        [alert addAction:[UIAlertAction actionWithTitle:buttonTitle style:UIAlertActionStyleDefault handler:handler(idx++)]];
    }
    
    switch (self.alertViewStyle)
    {
        case OHAlertViewStyleDefault:
            break;
        case OHAlertViewStylePlainTextInput: {
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.delegate = self;
            }];
        } break;
        case OHAlertViewStyleSecureTextInput: {
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.secureTextEntry = YES;
                textField.delegate = self;
            }];
        } break;
        case OHAlertViewStyleLoginAndPasswordInput: {
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.delegate = self;
            }];
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.secureTextEntry = YES;
                textField.delegate = self;
            }];
        } break;
        case OHAlertViewStyleEmailAndPasswordInput: {
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.keyboardType = UIKeyboardTypeEmailAddress;
                textField.delegate = self;
            }];
            [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
                textField.secureTextEntry = YES;
                textField.delegate = self;
            }];
        } break;
    }
    
    self.alert = alert;
    [self makeAlertRetainSelf];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(textFieldDidChange:) name:UITextFieldTextDidChangeNotification object:nil];
    [self textFieldDidChange:nil];
    
    // Show on topmost ViewController
    UIWindow* mainWindow = [[UIApplication sharedApplication] keyWindow] ?: [[[UIApplication sharedApplication] windows] lastObject];
    UIViewController* rootVC = mainWindow.rootViewController;
    UIViewController* topmostVC = rootVC;
    while (topmostVC != nil) {
        if (topmostVC.navigationController) topmostVC = topmostVC.navigationController.topViewController;
        if (topmostVC.presentedViewController == nil) break;
        topmostVC = topmostVC.presentedViewController;
    }
    [topmostVC presentViewController:alert animated:YES completion:nil];
#endif
}

-(NSInteger)numberOfButtons
{
    return self.otherButtonTitles.count + (self.cancelButtonTitle ? 1 : 0);
}

-(UITextField*)textFieldAtIndex:(NSInteger)textFieldIndex
{
#if USE_UIALERTVIEW
    return [self.alert textFieldAtIndex:textFieldIndex];
#else
    return self.alert.textFields[textFieldIndex];
#endif
}


/////////////////////////////////////////////////////////////////////////////
#pragma mark - Retain Cycle Managment

static void* kAssociatedAlertViewObject = &kAssociatedAlertViewObject;

-(void)makeAlertRetainSelf
{
    objc_setAssociatedObject(self.alert, &kAssociatedAlertViewObject, self, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

/////////////////////////////////////////////////////////////////////////////
#pragma mark - Delegate Methods

#if USE_UIALERTVIEW

-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (self.buttonHandler)
    {
		self.buttonHandler(self,buttonIndex);
	}
}

-(BOOL)alertViewShouldEnableFirstOtherButton:(UIAlertView *)alertView
{
    BOOL enable;
    if (self.shouldEnableFirstButton) {
        enable = self.shouldEnableFirstButton((OHAlertView *)alertView);
    } else {
        enable = YES;
    }
    return enable;
}

#else

-(void)textFieldDidChange:(NSNotification*)notif
{
    UIAlertAction* firstNonCancelAction = self.firstOtherButtonIndex >= 0 ? self.alert.actions[self.firstOtherButtonIndex] : nil;
    if (self.shouldEnableFirstButton && firstNonCancelAction)
    {
        firstNonCancelAction.enabled = self.shouldEnableFirstButton(self);
    }
}

#endif

@end
