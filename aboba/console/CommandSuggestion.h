// CommandSuggestion.h
#ifndef COMMANDSUGGESTION_H
#define COMMANDSUGGESTION_H

#include <QString>
#include <QStringList>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>

class CommandSuggestion {
public:
    CommandSuggestion(QListWidget *widget);
    void updateSuggestions(const QString &prefix, const QStringList &commands);
    void moveSelection(int offset);
    void hideSuggestions();
    QString currentPrefix(const QString &fullText, int promptPosition);
    void showSuggestions(const QRect &cursorRect, QWidget *parent);
    void updateGhostText(const QString &prefix, const QStringList &commands, QString &ghostTextOut);
    QListWidget* suggestionsWidget() const;

private:
    QListWidget *m_widget;
    int m_selectedSuggestion = -1;
};

class CleanItemDelegate : public QStyledItemDelegate {
    public:
        using QStyledItemDelegate::QStyledItemDelegate;
        
        void paint(QPainter* painter, const QStyleOptionViewItem& option, 
                 const QModelIndex& index) const override {
            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);
            
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            
            const bool isFirst = (index.row() == 0);
            const bool isLast = (index.row() == index.model()->rowCount() - 1);
            const qreal radius = 4.0;
    
            QRect rect = opt.rect.adjusted(0, 0, 1, 1);
            QPainterPath path;
            
            if (isFirst && isLast) {
                path.addRoundedRect(QRectF(rect), radius, radius);
            } 
            else if (isFirst) {
                path.moveTo(rect.bottomLeft());
                path.arcTo(rect.left(), rect.top(), radius*2, radius*2, 180, -90);
                path.lineTo(rect.right() - radius, rect.top());
                path.arcTo(rect.right() - radius*2, rect.top(), radius*2, radius*2, 90, -90);
                path.lineTo(rect.right(), rect.bottom());
                path.lineTo(rect.left(), rect.bottom());
            } 
            else if (isLast) {
                path.moveTo(rect.topLeft());
                path.lineTo(rect.right(), rect.top());
                path.lineTo(rect.right(), rect.bottom() - radius);
                path.arcTo(rect.right() - radius*2, rect.bottom() - radius*2, radius*2, radius*2, 0, -90);
                path.lineTo(rect.left() + radius, rect.bottom());
                path.arcTo(rect.left(), rect.bottom() - radius*2, radius*2, radius*2, 270, -90);
            } 
            else {
                path.addRect(rect);
            }
    
            painter->fillPath(path, QColor("#000000"));
            
            QString textToDraw = opt.text;
            int offsetX = 8;
            if (opt.state & QStyle::State_Selected) {
            textToDraw = "> " + textToDraw;
            offsetX += 3;
            painter->fillPath(path, QColor("#101010"));
            }
    
            painter->setPen(Qt::white);
            QFontMetrics fm(opt.font);
            int textHeight = fm.height();
            int textY = rect.top() + (rect.height() - textHeight) / 2 + fm.ascent();  

            painter->drawText(QPoint(rect.left() + offsetX, textY), textToDraw);
        }

        QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const override {
            return QSize(0, 20);
        }
    };
#endif // COMMANDSUGGESTION_H