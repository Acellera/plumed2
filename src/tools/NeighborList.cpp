/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2012 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.0.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#include <vector>
#include <algorithm>
#include "Vector.h"
#include "Pbc.h"
#include "AtomNumber.h"
#include "Tools.h"
#include "NeighborList.h"

namespace PLMD{
using namespace std;

NeighborList::NeighborList(const vector<AtomNumber>& list0, const vector<AtomNumber>& list1,
                           const bool& do_pair, const bool& do_pbc, const Pbc& pbc,
                           const double& distance, const unsigned& stride):
                           do_pair_(do_pair), do_pbc_(do_pbc), pbc_(pbc),
                           distance_(distance), stride_(stride)
{
// store full list of atoms needed
 fullatomlist_=list0;
 fullatomlist_.insert(fullatomlist_.end(),list1.begin(),list1.end());
 nlist0_=list0.size();
 nlist1_=list1.size();
 twolists_=true;
 if(!do_pair){
  nallpairs_=nlist0_*nlist1_;
 }else{
  plumed_assert(nlist0_==nlist1_);
  nallpairs_=nlist0_;
 }
 initialize();
 lastupdate_=0;
}

NeighborList::NeighborList(const vector<AtomNumber>& list0, const bool& do_pbc,
                           const Pbc& pbc, const double& distance,
                           const unsigned& stride):
                           do_pbc_(do_pbc), pbc_(pbc),
                           distance_(distance), stride_(stride){
 fullatomlist_=list0;
 nlist0_=list0.size();
 twolists_=false;
 nallpairs_=nlist0_*(nlist0_-1)/2;
 initialize();
 lastupdate_=0;
}

NeighborList::NeighborList(const NeighborList&nl) :
  do_pair_(nl.do_pair_),
  do_pbc_(nl.do_pbc_),
  twolists_(nl.twolists_),
  pbc_(nl.pbc_),
  fullatomlist_(nl.fullatomlist_),
  requestlist_(nl.requestlist_),
  neighbors_(nl.neighbors_),
  distance_(nl.distance_),
  stride_(nl.stride_),
  nlist0_(nl.nlist0_),
  nlist1_(nl.nlist1_),
  nallpairs_(nl.nallpairs_),
  lastupdate_(nl.lastupdate_)
{
}

// since this class contain references members, the only way to
// implement an assignment operator is via placement new.
// See http://cplusplus.co.il/2009/09/04/implementing-assignment-operator-using-copy-constructor/
NeighborList& NeighborList::operator=(const NeighborList&nl){
  if (this != &nl) {
      this->NeighborList::~NeighborList(); // explicit non-virtual destructor
      new (this) NeighborList(nl); // placement new
  }
  return *this;
}

void NeighborList::initialize() {
 neighbors_.clear();
 for(unsigned int i=0;i<nallpairs_;++i){
   neighbors_.push_back(getIndexPair(i));
 }
}

vector<AtomNumber>& NeighborList::getFullAtomList() {
 return fullatomlist_;
}

pair<unsigned,unsigned> NeighborList::getIndexPair(unsigned ipair) {
 pair<unsigned,unsigned> index;
 if(twolists_ && do_pair_){
  index=pair<unsigned,unsigned>(ipair,ipair+nlist0_);
 }else if (twolists_ && !do_pair_){
  index=pair<unsigned,unsigned>(ipair/nlist1_,ipair%nlist1_+nlist0_);
 }else if (!twolists_){
  unsigned ii = nallpairs_-1-ipair;
  unsigned  K = unsigned(floor((sqrt(double(8*ii+1))+1)/2));
  unsigned jj = ii-K*(K-1)/2;
  index=pair<unsigned,unsigned>(nlist0_-1-K,nlist0_-1-jj);
 }
 return index;
}

void NeighborList::update(const vector<Vector>& positions) {
 neighbors_.clear();
// check if positions array has the correct length 
 plumed_assert(positions.size()==fullatomlist_.size());
 for(unsigned int i=0;i<nallpairs_;++i){
   pair<unsigned,unsigned> index=getIndexPair(i);
   unsigned index0=index.first;
   unsigned index1=index.second;
   Vector distance;
   if(do_pbc_){
    distance=pbc_.distance(positions[index0],positions[index1]);
   } else {
    distance=delta(positions[index0],positions[index1]);
   }
   double value=distance.modulo();
   if(value<=distance_) {neighbors_.push_back(index);} 
 }
 setRequestList();
}

void NeighborList::setRequestList() {
 requestlist_.clear();
 for(unsigned int i=0;i<size();++i){
  requestlist_.push_back(fullatomlist_[neighbors_[i].first]);
  requestlist_.push_back(fullatomlist_[neighbors_[i].second]);
 }
 Tools::removeDuplicates(requestlist_);
}

vector<AtomNumber>& NeighborList::getReducedAtomList() {
 std::vector< pair<unsigned,unsigned> > newneighbors;
 for(unsigned int i=0;i<size();++i){
  unsigned newindex0=0,newindex1=0;
  AtomNumber index0=fullatomlist_[neighbors_[i].first];
  AtomNumber index1=fullatomlist_[neighbors_[i].second];
  for(unsigned j=0;j<requestlist_.size();++j){
   if(requestlist_[j]==index0) newindex0=j;
   if(requestlist_[j]==index1) newindex1=j;
  }
  newneighbors.push_back(pair<unsigned,unsigned>(newindex0,newindex1));
 }
 neighbors_.clear();
 neighbors_=newneighbors;
 return requestlist_;
}

unsigned NeighborList::getStride() const {
 return stride_;
}

unsigned NeighborList::getLastUpdate() const {
 return lastupdate_;
}

void NeighborList::setLastUpdate(unsigned step) {
 lastupdate_=step;
}

unsigned NeighborList::size() const {
 return neighbors_.size();
}

 pair<unsigned,unsigned> NeighborList::getClosePair(unsigned i) const {
 return neighbors_[i];
}

vector<unsigned> NeighborList::getNeighbors(unsigned index) {
 vector<unsigned> neighbors;
 for(unsigned int i=0;i<size();++i){
  if(neighbors_[i].first==index)  neighbors.push_back(neighbors_[i].second);
  if(neighbors_[i].second==index) neighbors.push_back(neighbors_[i].first);
 }
 return neighbors;
}

}