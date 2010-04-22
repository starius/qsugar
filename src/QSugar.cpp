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


#include "QSugar.hpp"


const QSugarVariantList QLIST;
const QSugarVariantMap QDICT;
const QDomDocument _QXML;


void buildDom(QDomElement element, const QVariant & value)
{
    switch ( value.type() )
    {
        case QVariant::List:
        {
            element.setAttribute("valuetype", "list");
            foreach ( QVariant it, value.toList() )
            {
                QDomElement li = element.ownerDocument().createElement("li");
                element.appendChild(li);
                buildDom(li, it);
            }
        }
            break;
        
        case QVariant::Map:
        {
            QVariantMap map = value.toMap();
            foreach ( QString key, map.keys() )
            {
                if ( key.isEmpty() ) // empty key designates body
                    buildDom(element, map[key]);
                else if ( key.startsWith("@") )
                    element.setAttribute(key.mid(1), map[key].toString());
                else
                {
                    QDomElement childElement = element.ownerDocument().createElement(key);
                    element.appendChild(childElement);
                    buildDom(childElement, map[key]);
                }
            }
        }
            break;
        
        default:
        {
            element.appendChild(
                element.ownerDocument().createTextNode(
                    value.toString()
                )
            );
        }
    }
}


QDomDocument operator<< (QDomDocument doc, QVariant var)
{
    buildDom(doc.documentElement(), var);
    return doc;
}


QDomDocument operator* (const QDomDocument &, const QString & code)
{
    QDomDocument doc;
    if ( code.indexOf(QRegExp("[<&]")) >= 0 )
        doc.setContent(code);
    else
        doc.appendChild(doc.createElement(code));
    return doc;
}


QSugarDomDocument operator< (QDomDocument doc, const QString & key)
{
    QSugarDomDocument sugardoc(doc);
    sugardoc.pendingKey = key;
    return sugardoc;
}


QDomDocument operator> (QDomDocument doc, const QString & value)
{
    doc.documentElement().appendChild(doc.createTextNode(value));
    return doc;
}


QDomDocument operator> (QDomDocument ldoc, const QDomDocument & rdoc)
{
    ldoc.documentElement().appendChild(rdoc);
    return ldoc;
}
