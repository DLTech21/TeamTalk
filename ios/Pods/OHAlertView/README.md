[![Version](http://cocoapod-badges.herokuapp.com/v/OHAlertView/badge.png)](http://cocoadocs.org/docsets/OHAlertView)
[![Platform](http://cocoapod-badges.herokuapp.com/p/OHAlertView/badge.png)](http://cocoadocs.org/docsets/OHAlertView)


## About this class

This class make it easier to use Alert Views with blocks, and make the transition between `UIAlertView` (iOS < 8.0) to UIAlertController (iOS 8.0+)

This allows you to provide directly the code to execute (as a block) in return to the tap on a button,
instead of declaring a delegate and implementing the corresponding methods.

This also has the huge advantage of **simplifying the code especially when using multiple `UIAlertViews`** in the same object (as in such case, it is not easy to have a clean code if you share the same delegate)

_Note: You may also be interested in [OHActionSheet](https://github.com/AliSoftware/OHActionSheet)_

## Usage Example

    [OHAlertView showAlertWithTitle:@"Alert Demo"
                            message:@"You like this sample?"
                       cancelButton:@"No"
                       otherButtons:@[@"Yes"]
                      buttonHandler:^(OHAlertView* alert, NSInteger buttonIndex)
     {
         NSLog(@"button tapped: %d",buttonIndex);
     
         if (buttonIndex == alert.cancelButtonIndex) {
             NSLog(@"No");
         } else {
             NSLog(@"Yes");
         }
     }];
     

## CocoaPods

This class is referenced in CocoaPods, so you can simply add `pod OHAlertView` to your Podfile to add it to your pods.

## Compatibility Notes

* This class uses blocks, which is a feature introduced in iOS 4.0.
* This class uses ARC.
* This class internally uses `UIAlertView` if you are targeting iOS < 8.0, and use `UIAlertController` if you are targeting iOS8 or above

## License

This code is under MIT License.
