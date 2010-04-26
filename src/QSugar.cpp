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


QDomDocument QSugarDomDocument::operator> (const QVariant & value)
{
    if ( pendingKey.isEmpty() )
        buildDom(documentElement(), value);
    else if ( pendingKey.startsWith("@") ) // attribute
        documentElement().setAttribute(pendingKey.mid(1), value.toString());
    else
    {
        QDomElement element = createElement(pendingKey);
        documentElement().appendChild(element);
        buildDom(element, value);
    }
    pendingKey.clear();
    return *dynamic_cast<QDomDocument*>(this);
}


QDomDocument QSugarDomDocument::operator> (QDomDocument doc)
{
    if ( pendingKey.isEmpty() )
        documentElement().appendChild(doc);
    else
    {
        QDomElement element = createElement(pendingKey);
        documentElement().appendChild(element);
        element.appendChild(doc);
    }
    pendingKey.clear();
    return *dynamic_cast<QDomDocument*>(this);
}



QDomDocument operator> (QDomDocument doc, QVariant var)
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


QDomDocument operator> (QDomDocument ldoc, const QDomDocument & rdoc)
{
    ldoc.documentElement().appendChild(rdoc);
    return ldoc;
}


QVariant takeApartDom(QDomElement el)
{
    QString valuetype = el.attribute("valuetype", "map");
    
    if ( valuetype == "map" )
    {
        QVariantMap varmap;
        QString body;
        
        QDomNamedNodeMap attributes = el.attributes();
        for ( int i = 0; i < attributes.count(); ++i )
        {
            QDomAttr attr = attributes.item(i).toAttr();
            varmap << ("@" + attr.name()) >> attr.value();
        }
        
        for ( QDomNode node = el.firstChild(); ! node.isNull(); node = node.nextSibling() )
        {
            if ( node.isElement() )
            {
                QDomElement child = node.toElement();
                varmap << child.tagName() >> takeApartDom(child);
            }
            else if ( node.isText() )
            {
                body = node.toText().data();
            }
        }
        
        if ( varmap.size() ) // sub-elements exist
        {
            if ( ! body.isEmpty() )
                varmap << "" >> body;
            return varmap;
        }
        else
            return el.text();
    }
    else if ( valuetype == "list" )
    {
        QVariantList varlist;
        for ( QDomNode node = el.firstChild(); ! node.isNull(); node = node.nextSibling() )
        {
            if ( node.isElement() )
                varlist << takeApartDom(node.toElement());
        }
        
        return varlist;
    }
    
    return QVariant(); // empty
}


void operator>> (QDomDocument doc, QVariant & var)
{
    var = takeApartDom(doc.documentElement());
}
