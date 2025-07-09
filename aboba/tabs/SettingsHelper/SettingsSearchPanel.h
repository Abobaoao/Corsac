#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>

class SettingsSearchPanel : public QWidget {
    Q_OBJECT
public:
    explicit SettingsSearchPanel(QWidget* parent = nullptr);
    
    QLineEdit* searchInput() const { return searchInput_; }
    QToolButton* themesButton() const { return btnThemes_; }
    QToolButton* pluginsButton() const { return btnPlugins_; }
    
    void animateSearchWidth(int newWidth);

protected:
    void showEvent(QShowEvent* event) override;

private:
    void setupGlassBackground();
    void setupSearchControls();
    QToolButton* createIconButton(const QString& icon, const QString& tooltip);
    
    QLineEdit* searchInput_ = nullptr;
    QToolButton* btnThemes_ = nullptr;
    QToolButton* btnPlugins_ = nullptr;
    int originalSearchWidth_ = 0;
    
    
    static constexpr int kGlassHeight = 120;
    static constexpr int kInputHeight = 40;
    static constexpr int kMargin = 20;
    static constexpr int kIconSpacing = 10;
    static constexpr int kButtonRadius = 20;
    static constexpr int kSearchShrink = 10;
    
    const QString kSearchInputStyle = R"(
        QLineEdit {
            background-color: rgba(0,0,0,200);
            border: none;
            border-radius: 20px;
            padding: 10px 20px;
            color: white;
            font-size: 14px;
        }
        QLineEdit:focus { 
            outline: none; 
        }
    )";
};