#ifndef FILTERINFO_H
#define FILTERINFO_H
#include <QDialog>
#include <QTextBrowser>

class FilterInfo : public QTextBrowser
{
public:
    FilterInfo();
    void setContent(std::string filter);
private:
    void readHtml(QString filter);
    void centerWindow(QWidget* window);
};

#endif // FILTERINFO_H
