// Copyright 2010, Juri Syrowiecki
// 
// This file is part of QSugar.
// 
// QSugar is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// QSugar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with QSugar.  If not, see <http://www.gnu.org/licenses/>.


#include <QtGui>
#include <QtXml>

#include "QSugar.hpp"


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


bool test_QLIST_QMAP()
{
    // A list
    
    qDebug() << (
        QVariantList() << 1 << 2.0 << "3"
    );
    
    QVariantList varlist = QLIST << 1 << 2.0 << "3";
    
    qDebug() << prettyString(varlist);
    
    
    // A dictionary
    
    QVariantMap m;
    m["one"] = 1;
    m["two"] = 2.0;
    m["three"] = "3";
    qDebug() << m;
    
    m = QMAP
        << "one" >> 1
        << "two" >> 2.0
        << "three" >> "3";
    
    qDebug() << prettyString(m);
    
    // A more complicated example
    
    qDebug() << prettyString(QMAP
        << "one" >> 1
        << "two" >> 2.0
        << "three" >> "3"
        << "nested list" >> (QLIST
            << 4
            << 5.0
            << QVariant(QLIST
                << "list"
                << "in a list"
            )
        )
        << "nested dictionary" >> (QMAP
            << "six" >> "6"
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
        > (QXML"head"
            <"title"> "Test page")
        > (QXML"body"
            <"h1"> "Hello world!"
            > (QXML"ul"
                <"li"> (QXML"a"
                    <"@href"> "http://cblp.su/"
                    > "cblp's page"
                )
                <"li"> (QXML"a"
                    <"@href"> "http://qt.nokia.com/"
                    > "Qt home"
                )
            )
        );
    
    qDebug() << sugarxml1.toString(4);
    
    /*
    QDomDocument sugarxml2 = (QMAP
        << "html" >> (QMAP
            << "head" >> (QMAP
                << "title" >> "Test page"
            )
            << "body" >> (QMAP
                << "h1" >> "Hello world!"
                << "ul" >> (QLIST
                    << (QMAP
                        << "li" >> (QMAP
                            << "a" >> (QMAP
                                << "@href" >> "http://cblp.su/"
                                << "" >> "cblp's page"
                            )
                        )
                    )
                    << (QMAP
                        << "li" >> (QMAP
                            << "a" >> (QMAP
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
                <"@foo"> "true"
                <"@bar"> "false"
        );
        
        qDebug() << doc.toString(4);
    }
    
    return true;
}


bool test_xmlLifelike()
{
    QDomDocument doc = QXML"rss";
    QDomDocument channel = QXML"channel";
    // for ( ... )
    {
        channel << (QXML"item"
            <"title"> "RSS test"
            <"link"> "http://bitbucket.org/cblp/qsugar/"
            <"description"> "Exporting RSS-like snippet of XML code using both classic QtXml approach and QXML pseudokeyword."
            <"author"> "cblp"
            <"pubDate"> QDateTime::currentDateTime()
        );
    }
    doc << channel; // Yes, QSugar is here, too!
    
    qDebug() << doc.toString(4);
    
    return true;
}


bool test_xmlRepresentation()
{
    QVariant var = (QMAP
        << "one" >> 1
        << "@two" >> 2.0
        << "three" >> "3"
        << "nested_list" >> (QLIST
            << 4
            << 5.0
            << QVariant(QLIST
                << "list"
                << "in a list"
            )
        )
        << "nested_dictionary" >> (QMAP
            << "six" >> "6"
        )
    );
    
    qDebug() << "var =" << var;
    
    QString xvar = (QXML"xvar" > var).toString(4);
    
    qDebug() << "xvar =" << xvar;
    
    QVariant var2;
    QXML xvar >> var2;
    
    qDebug() << "var2 =" << prettyString(var2);
    
    return true;
}


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    
    if ( argc <= 1 )
        return 1;
    
    switch ( argv[1][0] )
    {
    case '1':
        test_QLIST_QMAP();
        break;
    case '2':
        test_xmlExport();
        break;
    case '3':
        test_xmlModify();
        break;
    case '4':
        test_xmlLifelike();
        break;
    case '5':
        test_xmlRepresentation();
        break;
    }
    
    return 0;
}
