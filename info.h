#ifndef INFO_H
#define INFO_H
#include <QDialog>
#include <QTextBrowser>

class Info : public QTextBrowser
{
public:
    Info();
    void setContent(std::string content);
private:
    void readHtml(QString content);
    void centerWindow(QWidget* window);
};

#endif // INFO_H
