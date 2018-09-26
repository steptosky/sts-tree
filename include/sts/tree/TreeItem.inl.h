#pragma once

/*
**  Copyright(C) 2018, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

#include <cassert>

namespace sts {
namespace tree {

    /**************************************************************************************************/
    ////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Constructor init parent.
     * \param[in, out] inOutParent
     */
    template<typename TYPE>
    TreeItem<TYPE>::TreeItem(TreeItem * inOutParent) {
        TreeItem::setParent(static_cast<TYPE*>(inOutParent));
    }

    /*!
     * \details Destructor
     * \warning The destructor destroys all the item's children and removes this item from its parent.
     */
    template<typename TYPE>
    TreeItem<TYPE>::~TreeItem() {
        removeParent();
        TreeItem::deleteChildren();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Checks whether the item has a parent. No parent means the item is root.
     * \return True if the item is root otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::isRoot() const {
        return (mParent == nullptr);
    }

    /*!
     * \details Gets the tree hierarchy root (recursive by parents up to the root).
     * \return Root of the tree hierarchy.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::root() {
        auto currItem = static_cast<TYPE*>(this);
        do {
            if (currItem->isRoot()) {
                return currItem;
            }
            currItem = currItem->parent();
        } while (true);
    }

    /*!
     * \details Gets the tree hierarchy root (recursive by parents up to the root).
     * \return Root of the tree hierarchy.
     */
    template<typename TYPE>
    const TYPE * TreeItem<TYPE>::root() const {
        auto currItem = static_cast<TYPE*>(this);
        do {
            if (currItem->isRoot()) {
                return currItem;
            }
            currItem = currItem->parent();
        } while (true);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Sets the item's parent.
     * \remark You can set parent as the nullptr then the item will be as a root.
     * \param [in, out] inOutParent pointer to new parent.
     */
    template<typename TYPE>
    void TreeItem<TYPE>::setParent(TYPE * inOutParent) {
        if (mParent == inOutParent) {
            return;
        }
        if (inOutParent == nullptr) {
            removeParent();
        }
        else {
            inOutParent->appendChild(static_cast<TYPE*>(this));
        }
    }

    /*!
     * \details Gets the item's parent.
     * \return If the item has the parent then pointer to it otherwise nullptr.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::parent() {
        return mParent;
    }

    /*!
     * \details Gets the item's parent.
     * \return Pointer to item's parent or nullptr if the parent isn't set.
     */
    template<typename TYPE>
    const TYPE * TreeItem<TYPE>::parent() const {
        return mParent;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Gets the child by its index.
     * \param [in] index
     * \return Pointer to the child.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::operator[](const Index index) {
        assert(index < mChildren.size());
        return mChildren[index];
    }

    /*!
     * \details Gets the child by its index.
     * \param [in] index
     * \return Pointer to the child.
     */
    template<typename TYPE>
    const TYPE * TreeItem<TYPE>::operator[](const Index index) const {
        assert(index < mChildren.size());
        return mChildren[index];
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Gets the child by its index.
     * \param [in] index
     * \return Pointer to the child.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::childAt(const Index index) {
        return (*this)[index];
    }

    /*!
     * \details Gets the child by its index.
     * \param [in] index
     * \return Pointer to the child.
     */
    template<typename TYPE>
    const TYPE * TreeItem<TYPE>::childAt(const Index index) const {
        return (*this)[index];
    }

    /*!
     * \details Gets the item children's count.
     * \return Children's count.
     */
    template<typename TYPE>
    size_t TreeItem<TYPE>::childrenNum() const {
        return mChildren.size();
    }

    /*!
     * \details Searches an item index by its pointer.
     * \param [in] item pointer to tree item whose index must be found.
     * \return Index if specified pointer that was found otherwise \link TreeItem::npos \endlink.
     */
    template<typename TYPE>
    size_t TreeItem<TYPE>::indexOf(const TYPE * item) const {
        Index outIndex = 0;
        for (auto child : mChildren) {
            if (child == item) {
                return outIndex;
            }
            ++outIndex;
        }
        return npos;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::iterator TreeItem<TYPE>::begin() {
        return mChildren.begin();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::iterator TreeItem<TYPE>::end() {
        return mChildren.end();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::const_iterator TreeItem<TYPE>::begin() const {
        return mChildren.begin();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::const_iterator TreeItem<TYPE>::end() const {
        return mChildren.end();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::reverse_iterator TreeItem<TYPE>::rbegin() {
        return mChildren.rbegin();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::reverse_iterator TreeItem<TYPE>::rend() {
        return mChildren.rend();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::const_reverse_iterator TreeItem<TYPE>::rbegin() const {
        return mChildren.rbegin();
    }

    template<typename TYPE>
    typename TreeItem<TYPE>::Children::const_reverse_iterator TreeItem<TYPE>::rend() const {
        return mChildren.rend();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Inserts child by index.
     * \remark The Tree item takes ownership of the specified pointer.
     * \param [in] where index where new child must be inserted.
     * \param [in, out] inOutItem tree item that will be inserted as a child.
     * \return Pointer to the item which is the input parameter.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::insertChild(const Index where, TYPE * inOutItem) {
        assert(inOutItem);
        assert(where <= mChildren.size());
        assert(inOutItem->parent() != this);
        inOutItem->removeParent();
        inOutItem->mParent = static_cast<TYPE*>(this);
        mChildren.insert(mChildren.begin() + where, inOutItem);
        return inOutItem;
    }

    /*!
     * \details Adds a child to the and of the children list.
     * \remark The Tree item takes ownership of the specified pointer.
     * \param [in, out] inOutItem tree item that will be added as a child.
     * \return Pointer to the item which is the input parameter.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::appendChild(TYPE * inOutItem) {
        assert(inOutItem);
        assert(inOutItem->parent() != this);
        inOutItem->removeParent();
        inOutItem->mParent = static_cast<TYPE*>(this);
        mChildren.emplace_back(inOutItem);
        return inOutItem;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Takes the child by its index and removes it from the children list.
     *          The item will not be child's owner anymore, so don't forget to delete it yourself.
     * \param [in] index child index that must be removed from the children list and returned.
     * \return Item that is removed from the children list.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::takeChildAt(const Index index) {
        assert(index < mChildren.size());
        auto child = mChildren[index];
        eraseChild(mChildren.begin() + index);
        return child;
    }

    /*!
     * \details Just removes child from hierarchy. It doesn't destroy the item.
     * \param [in] position
     */
    template<typename TYPE>
    typename TreeItem<TYPE>::Children::iterator TreeItem<TYPE>::eraseChild(const typename Children::iterator position) {
        assert(position != mChildren.end());
        (*position)->mParent = nullptr;
        return mChildren.erase(position);
    }

    /*!
     * \details Removes child from item's children list by iterator.
     *          The child's destructor will be called while removing.
     * \param [in] position
     */
    template<typename TYPE>
    typename TreeItem<TYPE>::Children::iterator TreeItem<TYPE>::deleteChild(const typename Children::iterator position) {
        assert(position != mChildren.end());
        auto item = *position;
        auto out = eraseChild(position);
        delete item;
        return out;
    }

    /*!
     * \details Removes child from item's children list by child's index. 
     *          The child's destructor will be called while removing.
     * \param [in] index of a child that must be deleted.
     */
    template<typename TYPE>
    void TreeItem<TYPE>::deleteChild(const Index index) {
        assert(index < mChildren.size());
        delete takeChildAt(index);
    }

    /*!
     * \details Removes child from item's children list by child's pointer.
     *          The child's destructor will be called while removing.
     * \param [in, out] inOutItem pointer to a children that must be deleted.
     * \return True if the child by the specified pointer was deleted otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::deleteChild(TYPE * inOutItem) {
        for (auto iter = begin(); iter != end(); ++iter) {
            if (*iter == inOutItem) {
                deleteChild(iter);
                return true;
            }
        }
        return false;
    }

    /*!
     * \details Deletes <b>ALL</b> item's children.
     */
    template<typename TYPE>
    void TreeItem<TYPE>::deleteChildren() {
        for (auto child : mChildren) {
            // the children mustn't delete them-self from 
            // their parent as it doesn't make a sense in this case.
            child->mParent = nullptr;
            delete child;
        }
        mChildren.clear();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Checks whether the item has children.
     * \return True if the item has children otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::hasChildren() const {
        return !mChildren.empty();
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Checks whether the item is either branch or leaf.
     * \details If the item doesn't have any children then the item is leaf.
     * \return True if the item is leaf otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::isLeaf() const {
        return mChildren.empty();
    }

    /*!
     * \details Checks whether the item is either branch or leaf.
     * \details If the item has some children then the item is branch.
     * \return True if the item is branch otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::isBranch() const {
        return !mChildren.empty();
    }

    /*!
     * \details Check whether this item is one of the children of specified parent.
     * \note This method checks full hierarchy chain up to the root.
     * \param [in] parent a parent that you want to be checked.
     * \return True if this item has specified item as a parent otherwise false.
     */
    template<typename TYPE>
    bool TreeItem<TYPE>::isChildOf(const TYPE * parent) const {
        if (!mParent) {
            return false;
        }
        if (mParent == parent) {
            return true;
        }
        return mParent->isChildOf(parent);
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Clones the tree item.
     * \details default implementation is \code return nullptr; \endcode
     * \return Pointer to a cloned tree item.
     */
    template<typename TYPE>
    TYPE * TreeItem<TYPE>::clone() const {
        return nullptr;
    }

    /**************************************************************************************************/
    ///////////////////////////////////////////* Functions *////////////////////////////////////////////
    /**************************************************************************************************/

    /*!
     * \details Removes item's parent.
     */
    template<typename TYPE>
    void TreeItem<TYPE>::removeParent() {
        if (mParent != nullptr) {
            assert(mParent->indexOf(static_cast<TYPE*>(this)) != npos);
            removeFromContainer(&mParent->mChildren, this);
            mParent = nullptr;
        }
    }

    /*!
     * \details Finds and removes certain child from container without destruction.
     * \param [in, out] inOutContainer
     * \param [in] item
     */
    template<typename TYPE>
    void TreeItem<TYPE>::removeFromContainer(Children * inOutContainer, const TreeItem * item) {
        assert(inOutContainer);
        assert(item);
        for (auto it = inOutContainer->begin(); it != inOutContainer->end(); ++it) {
            if (*it == item) {
                inOutContainer->erase(it);
                return;
            }
        }
    }

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}
