//
//  OHAlertView.h
//  AliSoftware
//
//  Created by Olivier on 30/12/10.
//  Copyright 2010 AliSoftware. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, OHAlertViewStyle) {
    OHAlertViewStyleDefault = 0,
    OHAlertViewStyleSecureTextInput,
    OHAlertViewStylePlainTextInput,
    OHAlertViewStyleLoginAndPasswordInput,
    OHAlertViewStyleEmailAndPasswordInput, // same as previous but with email keyboard for login
};


@interface OHAlertView : NSObject

typedef void(^OHAlertViewButtonHandler)(OHAlertView* alert, NSInteger buttonIndex);
typedef BOOL (^OHAlertViewShouldEnableFirstOtherButton)(OHAlertView *alert);

@property (nonatomic, assign) OHAlertViewStyle alertViewStyle;
@property (nonatomic, copy) OHAlertViewShouldEnableFirstOtherButton shouldEnableFirstButton;

@property(nonatomic,readonly) NSInteger numberOfButtons;
@property(nonatomic,readonly) NSInteger cancelButtonIndex;
@property(nonatomic,readonly) NSInteger firstOtherButtonIndex;


/////////////////////////////////////////////////////////////////////////////

#pragma mark - Commodity Constructors


/**
 *	Create and immediately display an AlertView.
 *
 *	@param	title	The title of the AlertView (see UIAlertView)
 *	@param	message	The message of the AlertView (see UIAlertView)
 *	@param	cancelButtonTitle	The title for the "cancel" button (see UIAlertView)
 *	@param	otherButtonTitles	A NSArray of NSStrings containing titles for the other buttons (see UIAlertView)
 *	@param	handler	The block that will be executed when the user taps on a button. This block takes:
 *          - The OHAlertView as its first parameter, useful to get the firstOtherButtonIndex from it for example
 *          - The NSInteger as its second parameter, representing the index of the button that has been tapped
 */
+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
             cancelButton:(NSString *)cancelButtonTitle
             otherButtons:(NSArray *)otherButtonTitles
            buttonHandler:(OHAlertViewButtonHandler)handler;

/**
 *	Create and immediately display an AlertView with an alert style.
 *
 *	@param	title	The title of the AlertView (see UIAlertView)
 *	@param	message	The message of the AlertView (see UIAlertView)
  *	@param	alertStyle	The stlye of the AlertView (see UIAlertView)
 *	@param	cancelButtonTitle	The title for the "cancel" button (see UIAlertView)
 *	@param	otherButtonTitles	A NSArray of NSStrings containing titles for the other buttons (see UIAlertView)
 *	@param	handler	The block that will be executed when the user taps on a button. This block takes:
 *          - The OHAlertView as its first parameter, useful to get the firstOtherButtonIndex from it for example
 *          - The NSInteger as its second parameter, representing the index of the button that has been tapped
 */
+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
               alertStyle:(OHAlertViewStyle)alertStyle
             cancelButton:(NSString *)cancelButtonTitle
             otherButtons:(NSArray *)otherButtonTitles
            buttonHandler:(OHAlertViewButtonHandler)handler;


/**
 *	Create and immediately display an AlertView with only one button.
 *
 *	@param	title	The title of the AlertView (see UIAlertView)
 *	@param	message	The message of the AlertView (see UIAlertView)
 *	@param	dismissButtonTitle	The title for the only button, acting as a "cancel" button
 *
 *  @note This is a commodity method, equivalent of calling 
 *         `showAlertWithTitle:cancelButton:otherButtons:buttonHandler:` with `otherButtons`
 *         and `buttonHandler` set to `nil`.
 *  @note This method has no OHAlertViewButtonHandler parameter as it is intended to be used
 *        to display simply informational alerts.
 */
+(void)showAlertWithTitle:(NSString *)title
                  message:(NSString *)message
            dismissButton:(NSString *)dismissButtonTitle;


#pragma mark - Instance Methods

/**
 *	Create a new AlertView. Designed initializer.
 *
 *	@param	title	The title of the AlertView (see UIAlertView)
 *	@param	message	The message of the AlertView (see UIAlertView)
 *	@param	cancelButtonTitle	The title for the "cancel" button (see UIAlertView)
 *	@param	otherButtonTitles	A NSArray of NSStrings containing titles for the other buttons (see UIAlertView)
 *	@param	handler	The block that will be executed when the user taps on a button. This block takes:
 *          - The OHAlertView as its first parameter, useful to get the firstOtherButtonIndex from it for example
 *          - The NSInteger as its second parameter, representing the index of the button that has been tapped
 *	@return	The newly created AlertView. use the -show method of UIAlertView then to display it on screen.
 */
-(instancetype)initWithTitle:(NSString *)title
                     message:(NSString *)message
                cancelButton:(NSString *)cancelButtonTitle
                otherButtons:(NSArray *)otherButtonTitles
               buttonHandler:(OHAlertViewButtonHandler)handler;

/**
 * Retrieve a text field at an index.
 *
 * The field at index 0 will be the first text field (the single field or the login field), the field at index 1 will be the password field.
 *
 * @note raises NSRangeException when textFieldIndex is out-of-bounds.
 *
 */
-(UITextField*)textFieldAtIndex:(NSInteger)index;

/**
 * Display the alert on screen.
 */
-(void)show;

@end
