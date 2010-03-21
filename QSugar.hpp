#include <QtCore>
#include <QtXml>


class QSugarVariantList:
    public QVariantList
{
    
public:
    
    template <class T>
    QVariantList operator<< (const T & value) const
    {
        QVariantList temp(*this);
        temp << value;
        return temp;
    }
    
}; // class QSugarVariantList


class QSugarVariantMap:
    public QVariantMap
{
    
public:
    
    QSugarVariantMap()
    {
    }
    
    QSugarVariantMap(QVariantMap vmap)
    {
        *dynamic_cast<QVariantMap*>(this) = vmap;
    }
    
    /// if there is 1 element, it is the root
    /// otherwise, the root is &lt;root/&gt;
    QDomDocument toDom() const;
    
private:
    
    void buildDom(QDomElement & element, const QVariant & value) const;
    
}; // class QSugarVariantMap


class QSugarVariantMapCarrier
{
    
public:
    
    QSugarVariantMapCarrier(QSugarVariantMap vmap, const QString & key):
        vmap(vmap),
        pendingKey(key)
    {
    }
    
    template <class T>
    QSugarVariantMap operator>> (const T & value)
    {
        vmap.insert(pendingKey, value);
        return vmap;
    }
    
private:
    
    QSugarVariantMap vmap;
    QString pendingKey;
    
}; // class QSugarVariantMapCarrier


inline QSugarVariantMapCarrier operator<< (const QVariantMap & vmap, const QString & key)
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
    
    QDomDocument operator>> (const QString & value)
    {
        if ( pendingKey.isEmpty() )
            documentElement().appendChild(createTextNode(value));
        else if ( pendingKey.startsWith("@") ) // attribute
            documentElement().setAttribute(pendingKey.mid(1), value);
        else
        {
            QDomElement element = createElement(pendingKey);
            documentElement().appendChild(element);
            element.appendChild(createTextNode(value));
        }
        pendingKey.clear();
        return *dynamic_cast<QDomDocument*>(this);
    }
    
    QDomDocument operator>> (QDomDocument doc)
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
    
    /// synonym for operator>>()
    QDomDocument operator<< (QDomDocument doc)
    { return operator>>(doc); }
    
private:
    
    QString pendingKey;
    
//friends:
    
    friend QSugarDomDocument operator<< (QDomDocument doc, const QString & tagName);
    
}; // class QSugarDomDocument


/// Support for the QXML pseudokeyword.
/// @param code If it looks like XML (contains tags or XML-entities), then it will be parsed.
///     (See documentation about QDomDocument::setContent() for details.)
///     Otherwise, it is treated as root element name.
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

/// synonym
QDomDocument operator<< (QDomDocument ldoc, const QDomDocument & rdoc)
{ return ldoc >> rdoc; }


extern const QSugarVariantList QLIST;
extern const QSugarVariantMap QDICT;
extern const QDomDocument _QXML;
#define QXML _QXML*
