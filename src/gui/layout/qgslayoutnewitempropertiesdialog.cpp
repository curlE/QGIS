/***************************************************************************
                             qgslayoutnewitempropertiesdialog.cpp
                             ------------------------------------
    Date                 : July 2017
    Copyright            : (C) 2017 Nyall Dawson
    Email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgslayoutnewitempropertiesdialog.h"
#include "qgssettings.h"
#include "qgslayout.h"
#include "qgslayoutpagecollection.h"
#include "qgsgui.h"

#include <QButtonGroup>

QgsLayoutItemPropertiesDialog::QgsLayoutItemPropertiesDialog( QWidget *parent, Qt::WindowFlags flags )
  : QDialog( parent, flags )
{
  setupUi( this );

  QgsGui::enableAutoGeometryRestore( this );

  //make button exclusive
  QButtonGroup *buttonGroup = new QButtonGroup( this );
  buttonGroup->addButton( mUpperLeftRadioButton );
  buttonGroup->addButton( mUpperMiddleRadioButton );
  buttonGroup->addButton( mUpperRightRadioButton );
  buttonGroup->addButton( mMiddleLeftRadioButton );
  buttonGroup->addButton( mMiddleRadioButton );
  buttonGroup->addButton( mMiddleRightRadioButton );
  buttonGroup->addButton( mLowerLeftRadioButton );
  buttonGroup->addButton( mLowerMiddleRadioButton );
  buttonGroup->addButton( mLowerRightRadioButton );
  buttonGroup->setExclusive( true );

  QgsSettings settings;
  double lastWidth = settings.value( QStringLiteral( "LayoutDesigner/lastItemWidth" ), QStringLiteral( "50" ) ).toDouble();
  double lastHeight = settings.value( QStringLiteral( "LayoutDesigner/lastItemHeight" ), QStringLiteral( "50" ) ).toDouble();
  QgsUnitTypes::LayoutUnit lastSizeUnit = settings.enumValue( QStringLiteral( "LayoutDesigner/lastSizeUnit" ), QgsUnitTypes::LayoutMillimeters );
  setItemSize( QgsLayoutSize( lastWidth, lastHeight, lastSizeUnit ) );

  mPosUnitsComboBox->linkToWidget( mXPosSpin );
  mPosUnitsComboBox->linkToWidget( mYPosSpin );
  mSizeUnitsComboBox->linkToWidget( mWidthSpin );
  mSizeUnitsComboBox->linkToWidget( mHeightSpin );

  mLockAspectRatio->setWidthSpinBox( mWidthSpin );
  mLockAspectRatio->setHeightSpinBox( mHeightSpin );
}

void QgsLayoutItemPropertiesDialog::setItemPosition( QgsLayoutPoint position )
{
  // page number
  QPointF layoutPoint = mLayout->convertToLayoutUnits( position );
  int page = mLayout->pageCollection()->pageNumberForPoint( layoutPoint );

  // convert position to relative for current page
  position = mLayout->convertFromLayoutUnits( mLayout->pageCollection()->positionOnPage( layoutPoint ), position.units() );

  mPageSpin->setValue( page + 1 );
  whileBlocking( mPosUnitsComboBox )->setUnit( position.units() );
  mXPosSpin->setValue( position.x() );
  mYPosSpin->setValue( position.y() );
}

QgsLayoutPoint QgsLayoutItemPropertiesDialog::itemPosition() const
{
  return QgsLayoutPoint( mXPosSpin->value(), mYPosSpin->value(), mPosUnitsComboBox->unit() );
}

int QgsLayoutItemPropertiesDialog::page() const
{
  return mPageSpin->value() - 1;
}

void QgsLayoutItemPropertiesDialog::setItemSize( QgsLayoutSize size )
{
  mWidthSpin->setValue( size.width() );
  mHeightSpin->setValue( size.height() );
  whileBlocking( mSizeUnitsComboBox )->setUnit( size.units() );
}

QgsLayoutSize QgsLayoutItemPropertiesDialog::itemSize() const
{
  return QgsLayoutSize( mWidthSpin->value(), mHeightSpin->value(), mSizeUnitsComboBox->unit() );
}

QgsLayoutItem::ReferencePoint QgsLayoutItemPropertiesDialog::referencePoint() const
{
  if ( mUpperLeftRadioButton->isChecked() )
  {
    return QgsLayoutItem::UpperLeft;
  }
  else if ( mUpperMiddleRadioButton->isChecked() )
  {
    return QgsLayoutItem::UpperMiddle;
  }
  else if ( mUpperRightRadioButton->isChecked() )
  {
    return QgsLayoutItem::UpperRight;
  }
  else if ( mMiddleLeftRadioButton->isChecked() )
  {
    return QgsLayoutItem::MiddleLeft;
  }
  else if ( mMiddleRadioButton->isChecked() )
  {
    return QgsLayoutItem::Middle;
  }
  else if ( mMiddleRightRadioButton->isChecked() )
  {
    return QgsLayoutItem::MiddleRight;
  }
  else if ( mLowerLeftRadioButton->isChecked() )
  {
    return QgsLayoutItem::LowerLeft;
  }
  else if ( mLowerMiddleRadioButton->isChecked() )
  {
    return QgsLayoutItem::LowerMiddle;
  }
  else if ( mLowerRightRadioButton->isChecked() )
  {
    return QgsLayoutItem::LowerRight;
  }
  return QgsLayoutItem::UpperLeft;
}

void QgsLayoutItemPropertiesDialog::setReferencePoint( QgsLayoutItem::ReferencePoint point )
{
  switch ( point )
  {
    case QgsLayoutItem::UpperLeft:
      mUpperLeftRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::UpperMiddle:
      mUpperMiddleRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::UpperRight:
      mUpperRightRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::MiddleLeft:
      mMiddleLeftRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::Middle:
      mMiddleRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::MiddleRight:
      mMiddleRightRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::LowerLeft:
      mLowerLeftRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::LowerMiddle:
      mLowerMiddleRadioButton->setChecked( true );
      break;

    case QgsLayoutItem::LowerRight:
      mLowerRightRadioButton->setChecked( true );
      break;
  }
}

void QgsLayoutItemPropertiesDialog::setLayout( QgsLayout *layout )
{
  mSizeUnitsComboBox->setConverter( &layout->renderContext().measurementConverter() );
  mPosUnitsComboBox->setConverter( &layout->renderContext().measurementConverter() );
  mLayout = layout;
}
