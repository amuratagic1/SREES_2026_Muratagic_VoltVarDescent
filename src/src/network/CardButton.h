#pragma once
#include <gui/DrawableButton.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <td/LinePattern.h>

// Card-style button: icon on top, label on bottom, dotted rounded border.
class CardButton : public gui::DrawableButton
{
    gui::Image _img;
    gui::Image _imgValidated;
    bool       _hasValidatedImg;
    bool       _validated = false;
    bool       _alwaysAccent = false;
    bool       _stretchWidth = false;
    td::LinePattern _borderPattern = td::LinePattern::Dash;
    td::String _title;
    td::String _titleValidated;
    gui::Shape _border;
    float      _iconScale;
    bool       _iconLeft;

    void updateBorder(const gui::Size& sz)
    {
        const gui::CoordType inset = 2;
        gui::Rect r(inset, inset, sz.width - inset, sz.height - inset);
        _border.release();
        _border.createRoundedRect(r, 10, 2.0f, _borderPattern);
    }

protected:
    void measure(gui::CellInfo& ci) override
    {
        gui::DrawableButton::measure(ci);
        ci.minVer  = _size.height;
        ci.nResVer = 0;
        if (_stretchWidth)
            ci.nResHor = 1;
    }

    void reMeasure(gui::CellInfo& ci) override
    {
        gui::DrawableButton::reMeasure(ci);
        ci.minVer  = _size.height;
        ci.nResVer = 0;
        if (_stretchWidth)
            ci.nResHor = 1;
    }

    void onResize(const gui::Size& newSize) override
    {
        gui::DrawableButton::onResize(newSize);
        updateBorder(newSize);
    }

    void onDraw(const gui::Rect& rect) override
    {
        const bool accent = _validated || _alwaysAccent;
        auto borderColor = accent ? td::ColorID::ForestGreen : td::ColorID::SysText;
        _border.drawWire(borderColor);

        gui::Size sz;
        getSize(sz);

        const gui::CoordType padX = 8;
        auto& imgToDraw  = (accent && _hasValidatedImg) ? _imgValidated : _img;
        auto  textColor  = accent ? td::ColorID::ForestGreen : td::ColorID::SysText;
        const char* titleToDraw = (_validated && _titleValidated.length() > 0)
                                  ? _titleValidated.c_str() : _title.c_str();

        if (_iconLeft)
        {
            const gui::CoordType padY   = 8;
            const gui::CoordType innerH = sz.height - 2 * padY;
            const gui::CoordType iconSz = static_cast<gui::CoordType>(innerH * _iconScale);
            const gui::CoordType gap    = 8;

            gui::Size textSz;
            gui::Control::measure(titleToDraw, textSz);
            const gui::CoordType contentW = iconSz + gap + textSz.width;
            const gui::CoordType startX   = (sz.width - contentW) / 2;

            gui::Rect iconRect;
            iconRect.setOriginAndSize(startX, padY + (innerH - iconSz) / 2, iconSz, iconSz);
            imgToDraw.draw(iconRect, gui::Image::AspectRatio::Keep,
                           td::HAlignment::Center, td::VAlignment::Center);

            gui::Rect textRect;
            textRect.setOriginAndSize(startX + iconSz + gap, padY, textSz.width, innerH);
            gui::DrawableString::draw(titleToDraw, strlen(titleToDraw), textRect,
                                      gui::Font::ID::SystemNormal, textColor,
                                      td::TextAlignment::Left, td::VAlignment::Center);
        }
        else
        {
            const gui::CoordType padTop    = 12;
            const gui::CoordType padBottom = 10;
            const gui::CoordType textH     = 32;
            const gui::CoordType innerW    = sz.width - 2 * padX;
            const gui::CoordType iconH     = sz.height - padTop - textH - padBottom;
            const gui::CoordType sIconW    = static_cast<gui::CoordType>(innerW * _iconScale);
            const gui::CoordType sIconH    = static_cast<gui::CoordType>(iconH  * _iconScale);

            gui::Rect iconRect;
            iconRect.setOriginAndSize(padX + (innerW - sIconW) / 2,
                                      padTop + (iconH - sIconH) / 2,
                                      sIconW, sIconH);
            imgToDraw.draw(iconRect, gui::Image::AspectRatio::Keep,
                           td::HAlignment::Center, td::VAlignment::Center);

            gui::Rect textRect;
            textRect.setOriginAndSize(padX, padTop + iconH, innerW, textH);
            gui::DrawableString::draw(_title.c_str(), _title.length(), textRect,
                                      gui::Font::ID::SystemNormal, textColor,
                                      td::TextAlignment::Center, td::VAlignment::Center);
        }
    }

public:
    CardButton(const char* title, const char* imageResId, float iconScale = 1.0f,
               bool stretchWidth = false, td::UINT2 fixedHeight = 120,
               bool iconLeft = false, const char* validatedImgResId = nullptr,
               td::LinePattern borderPattern = td::LinePattern::Dash,
               bool alwaysAccent = false)
    : _img(imageResId)
    , _imgValidated(validatedImgResId ? validatedImgResId : imageResId)
    , _hasValidatedImg(validatedImgResId != nullptr)
    , _alwaysAccent(alwaysAccent)
    , _stretchWidth(stretchWidth)
    , _borderPattern(borderPattern)
    , _title(title)
    , _iconScale(iconScale)
    , _iconLeft(iconLeft)
    {
        _size.width  = 140;
        _size.height = fixedHeight;
        auto wLimit = stretchWidth ? gui::Control::Limit::UseAsMin : gui::Control::Limit::Fixed;
        setSizeLimits(140, wLimit, fixedHeight, gui::Control::Limit::Fixed);
        gui::Size sz(140, fixedHeight);
        updateBorder(sz);
    }

    void setValidated(bool v) { _validated = v; }

    void setValidatedTitle(const char* title) { _titleValidated = td::String(title); }

    void setAlwaysAccent(bool v) { _alwaysAccent = v; reDraw(); }

    void setBorderPattern(td::LinePattern pattern)
    {
        _borderPattern = pattern;
        gui::Size sz;
        getSize(sz);
        updateBorder(sz);
        reDraw();
    }
};
