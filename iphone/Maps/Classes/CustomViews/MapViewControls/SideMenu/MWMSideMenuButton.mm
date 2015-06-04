//
//  MWMSideMenuButton.m
//  Maps
//
//  Created by Ilya Grechuhin on 24.04.15.
//  Copyright (c) 2015 MapsWithMe. All rights reserved.
//

#import "MWMMapViewControlsCommon.h"
#import "MWMSideMenuButton.h"
#import "UIKitCategories.h"

@interface MWMSideMenuButton()

@property (nonatomic) CGRect defaultBounds;

@end

@implementation MWMSideMenuButton

- (instancetype)initWithCoder:(NSCoder *)aDecoder
{
  self = [super initWithCoder:aDecoder];
  if (self)
  {
    self.defaultBounds = self.bounds;
    self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  }
  return self;
}

- (void)setup
{
  self.contentScaleFactor = self.superview.contentScaleFactor;
  self.alpha = 0.0;
  [UIView animateWithDuration:framesDuration(3) animations:^
  {
    self.alpha = 1.0;
  }];
  UIImageView const * const animationIV = self.imageView;
  NSString const * const imageName = @"btn_green_menu_";
  [self setImage:[UIImage imageNamed:[imageName stringByAppendingString:@"1"]] forState:UIControlStateNormal];
  static NSUInteger const animationImagesCount = 4;
  NSMutableArray * const animationImages = [NSMutableArray arrayWithCapacity:animationImagesCount];
  for (NSUInteger i = 0; i < animationImagesCount; ++i)
    animationImages[i] = [UIImage imageNamed:[NSString stringWithFormat:@"%@%@", imageName, @(animationImagesCount - i)]];
  animationIV.animationImages = animationImages;
  animationIV.animationDuration = framesDuration(animationIV.animationImages.count);
  animationIV.animationRepeatCount = 1;
  [animationIV startAnimating];
  [self setNeedsLayout];
}

- (void)layoutSubviews
{
  [super layoutSubviews];
  self.bounds = self.defaultBounds;
  self.maxY = self.superview.height - 2.0 * kViewControlsOffsetToBounds;
  [self layoutXPosition:self.hidden];
}

- (void)layoutXPosition:(BOOL)hidden
{
  if (hidden)
    self.minX = self.superview.width;
  else
    self.maxX = self.superview.width - 2.0 * kViewControlsOffsetToBounds;
  
  m2::PointD const pivot(self.minX * self.contentScaleFactor - 2.0 * kViewControlsOffsetToBounds, self.maxY * self.contentScaleFactor - kViewControlsOffsetToBounds);
  [self.delegate setRulerPivot:pivot];
  [self.delegate setCopyrightLabelPivot:pivot];
}

#pragma mark - Properties

- (void)setHidden:(BOOL)hidden
{
  if (super.hidden == hidden)
    return;
  if (!hidden)
    super.hidden = NO;
  [self layoutXPosition:!hidden];
  [UIView animateWithDuration:framesDuration(kMenuViewHideFramesCount) animations:^
  {
    [self layoutXPosition:hidden];
  }
  completion:^(BOOL finished)
  {
    if (hidden)
      super.hidden = YES;
  }];
}

@end
