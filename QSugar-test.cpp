#include <QtGui>
#include <QtXml>

#include "QSugar.hpp"


QVariantList varlist;
QSugarVariantList sugarlist;
QSugarVariantMap sugarmap;


inline QString spaces(unsigned n)
{
    return QString("%1").arg("", n);
}


QString prettyString(QVariant qvar, unsigned indent = 0)
{
    const unsigned indentStep = 2;
    
    QString result;
    
    switch ( qvar.type() )
    {
        case QVariant::List:
        {
            result += "\n" + spaces(indent) + "[\n";
            foreach ( QVariant it, qvar.toList() )
                result += spaces(indent + indentStep) + prettyString(it, indent + indentStep) + ",\n";
            result += spaces(indent) + "]";
        }
            break;
        
        case QVariant::Map:
        {
            result += "\n" + spaces(indent) + "{\n";
            QVariantMap m = qvar.toMap();
            foreach ( QString k, m.keys() )
                result += spaces(indent + indentStep) + k + ": " + prettyString(m[k], indent + indentStep) + ",\n";
            result += spaces(indent) + "}";
        }
            break;
        
        default:
        {
            if ( qvar.canConvert<QString>() )
                result += qvar.toString();
            else
                result += QString("unconvertable ") + QVariant::typeToName(qvar.type());
        }
    }
    
    return result;
}


bool test_QLIST_QDICT()
{
    // A list
    
    qDebug() << (
        QVariantList() << 1 << 2.0 << "3"
    );
    
    varlist = QLIST << 1 << 2.0 << "3";
    
    qDebug() << prettyString(varlist);
    
    
    // A dictionary
    
    QVariantMap m;
    m["one"] = 1;
    m["two"] = 2.0;
    m["three"] = "3";
    qDebug() << m;
    
    sugarmap = QDICT
        << "one" >> 1
        << "two" >> 2.0
        << "three" >> "3";
    
    qDebug() << prettyString(sugarmap);
    
    // A more complicated example
    
    qDebug() << prettyString(
        QDICT
        << "one" >> 1
        << "two" >> 2.0
        << "three" >> "3"
        << "nested list" >> (
            QLIST
            << 4
            << 5.0
        )
        << "nested dictionary" >> (
            QDICT
            << "six" >> "6"
            << "seven" >> (
                QLIST
                << "nested list"
                << "in a nested dictionary"
            )
        )
    );
    
    
    // Simplified initialization
    
    /*/
    QWidget * w = new QWidget;
    QVBoxLayout * lay = new QVBoxLayout;
    {
        QLabel * label1 = new QLabel("QSugar 1", w);
        label1->setToolTip("QSugar test label 1");
        label1->setStyleSheet("font-weight: bold; color: lightgreen; background: black;");
        label1->setAlignment(Qt::AlignCenter);
        label1->setMargin(20);
        lay->addWidget(label1);
        
        lay->addWidget(
            QNEW<QLabel>(w)
            << "objectName" >> "label2"
            << "text" >> "QSugar 2"
            << "toolTip" >> "QSugar test label 2"
            << "styleSheet" >> "font-weight: bold; color: magenta; background: black;"
            << "margin" >> 20
        );
    }
    w->setLayout(lay);
    w->show();
    qApp->exec();
    //*/
    
    return true;
}


bool test_xmlExport()
{
    QDomDocument dom;
    {
        QDomElement html, head, title, body, h1, ul, li, a;
        dom.appendChild(html = dom.createElement("html"));
            html.appendChild(head = dom.createElement("head"));
                head.appendChild(title = dom.createElement("title"));
                    title.appendChild(dom.createTextNode("Test page"));
            html.appendChild(body = dom.createElement("body"));
                body.appendChild(h1 = dom.createElement("h1"));
                    h1.appendChild(dom.createTextNode("Hello world!"));
                body.appendChild(ul = dom.createElement("ul"));
                    ul.appendChild(li = dom.createElement("li"));
                        li.appendChild(a = dom.createElement("a"));
                            a.setAttribute("href", "http://cblp.su/");
                            a.appendChild(dom.createTextNode("cblp's page"));
                    ul.appendChild(li = dom.createElement("li"));
                        li.appendChild(a = dom.createElement("a"));
                            a.setAttribute("href", "http://qt.nokia.com/");
                            a.appendChild(dom.createTextNode("Qt home"));
    }
    
    qDebug() << dom.toString(4);
    
    QDomDocument sugarxml1 = QXML"html"
        >> (QXML"head"
            << "title" >> "Test page")
        >> (QXML"body"
            << "h1" >> "Hello world!"
            >> (QXML"ul"
                << "li" >> (QXML"a"
                    << "@href" >> "http://cblp.su/"
                    >> "cblp's page"
                )
                << "li" >> (QXML"a"
                    << "@href" >> "http://qt.nokia.com/"
                    >> "Qt home"
                )
            )
        );
    
    qDebug() << sugarxml1.toString(4);
    
    /*
    QDomDocument sugarxml2 = (QDICT
        << "html" >> (QDICT
            << "head" >> (QDICT
                << "title" >> "Test page"
            )
            << "body" >> (QDICT
                << "h1" >> "Hello world!"
                << "ul" >> (QLIST
                    << (QDICT
                        << "li" >> (QDICT
                            << "a" >> (QDICT
                                << "@href" >> "http://cblp.su/"
                                << "" >> "cblp's page"
                            )
                        )
                    )
                    << (QDICT
                        << "li" >> (QDICT
                            << "a" >> (QDICT
                                << "@href" >> "http://qt.nokia.com/"
                                << "" >> "Qt home"
                            )
                        )
                    )
                )
            )
        )
    ).toDom();
    
    qDebug() << sugarxml2.toString(4);
    //*/
    
    return true;
}


bool test_xmlModify()
{
    {
        QDomDocument doc;
        doc.setContent(QString("<root><alpha/></root>"));
        
        QDomElement elemB = doc.createElement("beta");
            elemB.setAttribute("foo", "true");
            elemB.setAttribute("bar", "false");
        doc.elementsByTagName("alpha").item(0).appendChild(elemB);
        
        qDebug() << doc.toString(4);
    }
    
    {
        QDomDocument doc = QXML"<root><alpha/></root>";
        
        doc.elementsByTagName("alpha").item(0).appendChild(
            QXML"beta"
                << "@foo" >> "true"
                << "@bar" >> "false"
        );
        
        qDebug() << doc.toString(4);
    }
    
    return true;
}


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    
    //test_QLIST_QDICT();
    test_xmlExport();
    //test_xmlModify();
    
    return 0;
}
