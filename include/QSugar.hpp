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


#ifndef QSugar_QSugar_hpp
#define QSugar_QSugar_hpp


#include <QtCore>
#include <QtXml>


class QSugarVariantMapCarrierConst
{
    
private:
    
    QVariantMap vmapcopy;
    QString pendingKey;
    
public:
    
    QSugarVariantMapCarrierConst(const QVariantMap & vmap, const QString & key):
        vmapcopy(vmap),
        pendingKey(key)
    {
    }
    
    template <class T>
    QVariantMap operator>> (const T & value)
    {
        vmapcopy.insert(pendingKey, value);
        return vmapcopy;
    }
    
}; // class QSugarVariantMapCarrier


class QSugarVariantMapCarrier
{
    
private:
    
    QVariantMap & vmap;
    QString pendingKey;
    
public:
    
    QSugarVariantMapCarrier(QVariantMap & vmap, const QString & key):
        vmap(vmap),
        pendingKey(key)
    {
    }
    
    template <class T>
    QVariantMap & operator>> (const T & value)
    {
        vmap.insert(pendingKey, value);
        return vmap;
    }
    
}; // class QSugarVariantMapCarrier


inline QSugarVariantMapCarrierConst operator<< (const QVariantMap & vmap, const QString & key)
{
    return QSugarVariantMapCarrierConst(vmap, key);
}


inline QSugarVariantMapCarrier operator<< (QVariantMap & vmap, const QString & key)
{
    return QSugarVariantMapCarrier(vmap, key);
}


class QSugarObjectCarrier
{
    
public:
    
    QSugarObjectCarrier(QObject * pobj, const QString & key = QString()):
        pobj(pobj),
        pendingKey(key)
    {
    }
    
    QSugarObjectCarrier & operator<< (QString key)
    {
        // check existance of property waiting for value
        Q_ASSERT_X(pobj->metaObject()->indexOfProperty(key.toUtf8()) >= 0, "QSugarObjectCarrier::operator<<", ("No such property: " + key).toUtf8());
        pendingKey = key;
        return (*this);
    }
    
    template <class T>
    QSugarObjectCarrier & operator>> (const T & value)
    {
        pobj->setProperty(pendingKey.toUtf8(), value);
        return (*this);
    }
    
    template <class T>
    operator T* () const
    {
        return dynamic_cast<T*>(pobj);
    }
    
private:
    
    QObject * pobj;
    QString pendingKey;
    
}; // QSugarObjectCarrier


template <class T>
inline QSugarObjectCarrier QNEW()
{
    return QSugarObjectCarrier(new T);
}

template <class T>
inline QSugarObjectCarrier QNEW(QObject * parent)
{
    return QSugarObjectCarrier(new T(parent));
}

template <class T>
inline QSugarObjectCarrier QNEW(QWidget * parent)
{
    return QSugarObjectCarrier(new T(parent));
}


class QSugarDomDocument:
    public QDomDocument
{
    
public:
    
    QSugarDomDocument()
    {
    }
    
    QSugarDomDocument(QDomDocument doc)
    {
        *dynamic_cast<QDomDocument*>(this) = doc;
    }
    
    QDomDocument operator> (const QVariant & value);
    
    QDomDocument operator> (QDomDocument doc);
    
    /// synonym for operator>()
    QDomDocument operator<< (QDomDocument doc)
    { return operator>(doc); }
    
private:
    
    QString pendingKey;
    
//friends:
    
    friend QSugarDomDocument operator< (QDomDocument doc, const QString & tagName);
    
}; // class QSugarDomDocument


/// Support for the QXML pseudokeyword.
/// @param code If it looks like XML (contains tags or XML-entities), then it will be parsed.
///     (See documentation about QDomDocument::setContent() for details.)
///     Otherwise, it is treated as root element name.
QDomDocument operator* (const QDomDocument &, const QString & code);


QSugarDomDocument operator< (QDomDocument doc, const QString & key);


QDomDocument operator> (QDomDocument ldoc, const QDomDocument & rdoc);

/// synonym
inline QDomDocument operator<< (QDomDocument ldoc, const QDomDocument & rdoc)
{ return ldoc > rdoc; }


QDomDocument operator> (QDomDocument doc, QVariant var);

/// synonym
QDomDocument operator<< (QDomDocument doc, QVariant var)
{ return doc > var; }


void operator>> (QDomDocument doc, QVariant & var);


#define QLIST (QVariantList())
#define QMAP (QVariantMap())
extern const QDomDocument _QXML;
#define QXML (_QXML)*


#endif // QSugar_QSugar_hpp
