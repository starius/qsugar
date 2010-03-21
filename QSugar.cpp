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
