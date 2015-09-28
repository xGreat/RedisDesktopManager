#include "namespaceitem.h"
#include "connections-tree/iconproxy.h"
#include <QMenu>

using namespace ConnectionsTree;

NamespaceItem::NamespaceItem(const QString &fullPath,
                             QSharedPointer<Operations> operations,
                             QWeakPointer<TreeItem> parent)
    : m_fullPath(fullPath),
      m_operations(operations),
      m_parent(parent),
      m_locked(false)
{
    m_displayName = m_fullPath.mid(m_fullPath.lastIndexOf(m_operations->getNamespaceSeparator())+1);
}

QString NamespaceItem::getDisplayName() const
{    
    return QString("%1 (%2)").arg(m_displayName).arg(childCount(true));
}

QString NamespaceItem::getName() const
{
    return m_displayName;
}

QIcon NamespaceItem::getIcon() const
{    
    return IconProxy::instance()->get(":/images/namespace.png");
}

QList<QSharedPointer<TreeItem> > NamespaceItem::getAllChilds() const
{
    return m_childItems;
}

uint NamespaceItem::childCount(bool recursive) const
{
    if (!recursive)
        return m_childItems.size();

    uint count = 0;
    for (auto item : m_childItems) {
        if (item->supportChildItems()) {
            count += item->childCount(true);
        } else {
            count += 1;
        }
    }
    return count;
}

QSharedPointer<TreeItem> NamespaceItem::child(uint row) const
{    
    if (row < m_childItems.size())
        return m_childItems.at(row);

    return QSharedPointer<TreeItem>();
}

QWeakPointer<TreeItem> NamespaceItem::parent() const
{
    return m_parent;
}

bool NamespaceItem::onClick(TreeItem::ParentView&)
{
    return true;
}

QSharedPointer<QMenu> NamespaceItem::getContextMenu(TreeItem::ParentView&)
{
    return QSharedPointer<QMenu>();
}

bool NamespaceItem::isLocked() const
{
    return m_locked;
}

bool NamespaceItem::isEnabled() const
{
    return true;
}

void NamespaceItem::append(QSharedPointer<TreeItem> item)
{
    if (typeid(NamespaceItem)==typeid(*item)) {
        m_childNamespaces[item.staticCast<NamespaceItem>()->getName()] = qSharedPointerCast<NamespaceItem>(item);
    }
    m_childItems.append(item);
}

QSharedPointer<NamespaceItem> NamespaceItem::findChildNamespace(const QString &name)
{
    if (!m_childNamespaces.contains(name))
        return QSharedPointer<NamespaceItem>();

    return m_childNamespaces[name];
}

