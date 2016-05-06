# LCActionSheet

[![Travis](https://img.shields.io/travis/LeoiOS/LCActionSheet.svg?style=flat)](https://travis-ci.org/LeoiOS/LCActionSheet)
[![CocoaPods](https://img.shields.io/cocoapods/v/LCActionSheet.svg)](http://cocoadocs.org/docsets/LCActionSheet)
[![CocoaPods](https://img.shields.io/cocoapods/l/LCActionSheet.svg)](https://raw.githubusercontent.com/LeoiOS/LCActionSheet/master/LICENSE)
[![CocoaPods](https://img.shields.io/cocoapods/p/LCActionSheet.svg)](http://cocoadocs.org/docsets/LCActionSheet)
[![LeoDev](https://img.shields.io/badge/blog-LeoDev.me-brightgreen.svg)](http://leodev.me)

一个简约优雅的 ActionSheet。微信和新浪微博也是采取类似的ActionSheet。

![image](https://github.com/LeoiOS/LCActionSheet/blob/master/LCActionSheetDemo.gif)

````
In me the tiger sniffs the rose.

心有猛虎，细嗅蔷薇。
````

欢迎访问 **我的博客**：http://LeoDev.me


## 前言 Foreword

OK，这次我是看系统的 UIActionSheet 不爽。不能更改 tintColor (蓝蓝的其实也还看得过去)就算了，风格还跟自己的 App 极为不搭。

然后看了看微信和新浪微博的 ActionSheet，嗯，还不错。于是自己搞了个 ActionSheet，并发扬大庇天下码农俱欢颜的精神，放上来给大家用:)



## 代码 Code

* 两个方法：
  - 方法一：[CocoaPods](https://cocoapods.org/) 导入：`pod 'LCActionSheet'`
  - 方法二：把 LCActionSheet 文件夹(在 Demo 中)拖到你的项目中

* 在相应位置导入头文件：`#import "LCActionSheet.h"`，遵守协议 `<LCActionSheetDelegate>`
* 调用下面的方法即可：

  ````objc
  // 1. 类方法 + Block
  LCActionSheet *sheet = [LCActionSheet sheetWithTitle:nil buttonTitles:@[@"拍照", @"从相册选择"] redButtonIndex:-1 clicked:^(NSInteger buttonIndex) {

      NSLog(@"> Block way -> Clicked Index: %ld", (long)buttonIndex);
  }];

  [sheet show];


  // 2. 实例方法 + Delegate + 添加按钮
  LCActionSheet *sheet = [[LCActionSheet alloc] initWithTitle:@"你确定要注销吗？"
                                                 buttonTitles:nil
                                               redButtonIndex:0
                                                     delegate:self];

  [sheet addButtonTitle:@"确定"];

  [sheet show];
  ````

* 监听方法 (代理方法，可选实现):

  ````objc
  - (void)actionSheet:(LCActionSheet *)actionSheet didClickedButtonAtIndex:(NSInteger)buttonIndex;
  ````

* 自定义实现 (By [zachgenius](https://github.com/zachgenius))
  ````objc
  LCActionSheet* sheet = [[LCActionSheet alloc] init];

  float version = [[[UIDevice currentDevice] systemVersion] floatValue];

  if (version < 8.0) {

      [sheet addButtonTitle:@"iOS 7.x"];

  } else {

      [sheet addButtonTitle:@"iOS 8+"];
  }

  sheet.clickedBlock = ^(NSInteger buttonIndex) {

      NSLog(@"Hello %ld!", (long)buttonIndex);
  };

  [sheet show];

  ````


## TODO

* 使用 CALayer 优化性能
* 使用约束布局
* 支持横排



## 更新日志 Update Logs

### 2016.03.07 (Tag: 1.2.0)

* 合并 [PR](https://github.com/LeoiOS/LCActionSheet/pull/14) by [apache2046](https://github.com/apache2046)，致谢！

  > Swift bug fixed
  >
  > mainBundle 这种方法无法在将 LCActionSheet 作为 Framework 时正确找到资源包路径


### 2016.02.17 (Tag: 1.1.5)

* 合并 [PR](https://github.com/LeoiOS/LCActionSheet/pull/11) by [nix1024](https://github.com/nix1024)，致谢！

  > Add background opacity & animation duration option
  >
  > 添加暗黑背景透明度和动画持续时间的设定


### 2015.12.16 (Tag: 1.1.3)

* 合并 [PR](https://github.com/LeoiOS/LCActionSheet/pull/9) by [zachgenius](https://github.com/zachgenius)，致谢！

  > 增加了一些功能实现，如增加自定义添加按钮的方法，增加按钮本地化，增加自定义按钮颜色，并且优化逻辑。

* V 1.1.2 被怪物吃掉了！👹


### 2015.12.09 (Tag: 1.1.1)

* 标题支持最多两行。两行时会适当调整标题的背景高度。


### 2015.12.07 (Tag: 1.1.0)

* 要 Block？满足你！

* 优化逻辑：创建 ActionSheet 时，不再添加到 window 上。


### 2015.11.09 (Tag: 1.0.6)

* 添加对 [CocoaPods](https://cocoapods.org/) 的支持：

  ````objc
  pod 'LCActionSheet'
  ````


### 2015.05.08 (Tag: 1.0.0)

* 修复：新添加的 \_backWindow 在某些情况下导致界面无反应的BUG。——by [kuanglijun312](https://github.com/kuanglijun312)


### 2015.05.08 (Tag: 1.0.0)

* 修复：当 StatusBarStyle 为 UIStatusBarStyleLightContent 时，背景不会遮挡 statusBar 的问题。——by [陈威](https://github.com/weiwei1035)


### 2015.05.05 (Tag: 1.0.0)

* 我还是没有适配横屏(´Д｀)
* 增加了类方法，可以通过类方法实例化 actionSheet。
* 完善部分注释。



## 提示 Tips

- LCActionSheet 是添加到当前的 Window 上，没适配横屏。
- 提供了 title、buttons、redButton、cancelBtn 这些杂七杂八的东东，应该全了。
- buttonIndex 从上到下从 0 依次递增。如果不想有 redButton，在 `redButtonIndex:` 处传 `-1` 即可。
- 协议 `<LCActionSheetDelegate>` 能监听到点击的按钮的 index，这个方法是可选实现的。


## 鸣谢 Thanks

海纳百川，有容乃大。感谢开源社区和众攻城狮的支持！感谢众多 issues 和 pr！期待你的 [pr](https://github.com/LeoiOS/LCActionSheet/pulls)！


## 示例 Examples

![image](https://github.com/LeoiOS/LCActionSheet/blob/master/01.png)
---
![image](https://github.com/LeoiOS/LCActionSheet/blob/master/02.png)
---


## 联系 Support

* 发现问题请 [Issues](https://github.com/LeoiOS/LCActionSheet/issues/new) 我，谢谢:)
* Mail: devtip@163.com
* Blog: http://LeoDev.me
* 土豪捐赠通道: 👇

![Alipay](http://7xl8ia.com1.z0.glb.clouddn.com/alipay.png)



## 授权 License

本项目采用 [MIT license](http://opensource.org/licenses/MIT) 开源，你可以利用采用该协议的代码做任何事情，只需要继续继承 MIT 协议即可。
