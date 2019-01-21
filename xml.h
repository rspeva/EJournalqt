#ifndef XML_H
#define XML_H

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QFile>

class Xml
{
public:
    Xml();
public slots:
    void createXML();
private:
    QDomElement lesson(QDomDocument&, const QString&, const QString&,const QString&);
    QDomElement makeElement(QDomDocument&, const QString&, const QString&,const QString&);
};

#endif // XML_H
