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


QDomDocument QSugarVariantMap::toDom() const
{
    QDomDocument doc;
    
    if ( isEmpty() )
        return doc; // empty QDomDocument
    
    QDomElement root;
    
    if ( count() == 1 )
    {
        QString tagName = begin().key();
        root = doc.createElement(tagName);
        doc.appendChild(root);
        buildDom(root, value(tagName));
    }
    else
    {
        root = doc.createElement("root");
        doc.appendChild(root);
        buildDom(root, *this);
    }
    
    return doc;
}


void QSugarVariantMap::buildDom(QDomElement & element, const QVariant & value) const
{
    switch ( value.type() )
    {
        case QVariant::List:
        {
            foreach ( QVariant it, value.toList() )
                buildDom(element, it);
        }
            break;
        
        case QVariant::Map:
        {
            QVariantMap map = value.toMap();
            foreach ( QString key, map.keys() )
            {
                if ( key.isEmpty() )
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
/*    foreach ( QString key, keys() )
    {
    }*/
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


QSugarDomDocument operator<< (QDomDocument doc, const QString & key)
{
    QSugarDomDocument sugardoc(doc);
    sugardoc.pendingKey = key;
    return sugardoc;
}


QDomDocument operator>> (QDomDocument doc, const QString & value)
{
    doc.documentElement().appendChild(doc.createTextNode(value));
    return doc;
}


QDomDocument operator>> (QDomDocument ldoc, const QDomDocument & rdoc)
{
    ldoc.documentElement().appendChild(rdoc);
    return ldoc;
}
