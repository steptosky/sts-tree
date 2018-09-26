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

#include <cstddef>
#include <vector>

namespace sts {
namespace tree {

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/

    /*!
     * \details Tree Item.
     * \pre
     *      - You must not use this class directly.
     *      - The tree item is owner of its children.
     *  When tree item is being destroyed it destroys all its children and remove itself from its parent.
     * 
     * \tparam TYPE your type.
     */
    template<typename TYPE>
    class TreeItem {
    public:

        typedef std::vector<TYPE*> Container;
        typedef std::size_t Index;    /*!< \details Tree item index. */
        const Index npos = Index(-1); /*!< \details Means no position. */
        typedef Container Children;

        //---------------------------------------------------------------
        /// @{ 

        TreeItem() = default;
        explicit TreeItem(TreeItem * inOutParent);
        virtual ~TreeItem();

        /// @}
        //---------------------------------------------------------------
        /// @{

        bool isRoot() const;
        TYPE * root();
        const TYPE * root() const;

        /// @}
        //---------------------------------------------------------------
        /// @{

        virtual void setParent(TYPE * inOutParent);
        TYPE * parent();
        const TYPE * parent() const;

        /// @}
        //---------------------------------------------------------------
        /// @{

        TYPE * operator[](Index index);
        const TYPE * operator[](Index index) const;

        TYPE * childAt(Index index);
        const TYPE * childAt(Index index) const;

        Index childrenCount() const;
        const Children & children() const;
        virtual TYPE * takeChildAt(Index index);

        /// @}
        //---------------------------------------------------------------
        /// @{

        typename Children::iterator begin();
        typename Children::iterator end();

        typename Children::const_iterator begin() const;
        typename Children::const_iterator end() const;

        typename Children::reverse_iterator rbegin();
        typename Children::reverse_iterator rend();

        typename Children::const_reverse_iterator rbegin() const;
        typename Children::const_reverse_iterator rend() const;

        /// @}
        //---------------------------------------------------------------
        /// @{

        virtual TYPE * prependChild(TYPE * inOutItem);
        virtual TYPE * insertChild(Index where, TYPE * inOutItem);
        virtual TYPE * appendChild(TYPE * inOutItem);

        /// @}
        //---------------------------------------------------------------
        /// @{

        virtual void deleteChildAt(Index index);
        virtual bool deleteChild(TYPE * inOutItem);
        virtual void deleteChildren();

        /// @}
        //---------------------------------------------------------------
        /// @{

        bool hasChildren() const;
        Index indexOf(const TYPE * item) const;

        /// @}
        //---------------------------------------------------------------
        /// @{

        bool isLeaf() const;
        bool isBranch() const;
        bool isChildOf(const TYPE * parent) const;

        /// @}
        //---------------------------------------------------------------
        /// @{

        virtual TYPE * clone() const;

        /// @}
        //---------------------------------------------------------------

    protected:

        Children & childrenAccess();

    private:

        TYPE * mParent = nullptr;
        Children mChildren;

        void removeFromContainer(Children * inOutContainer, const TreeItem * item);
        void removeParent();

    };

    /********************************************************************************************************/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /********************************************************************************************************/
}
}

#include "TreeItem.inl.h"
