
// ============================================================================
//
// Copyright (c) 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision:  $
// release_date  : $CGAL_Date:  $
//
// file          : include/CGAL/Segment_2_Iso_rectangle_2_intersection.h
// source        : intersection_2_2.fw
// author(s)     : Geert-Jan Giezeman
//
// coordinator   : Saarbruecken
//
// ============================================================================


#ifndef CGAL_SEGMENT_2_ISO_RECTANGLE_2_INTERSECTION_H
#define CGAL_SEGMENT_2_ISO_RECTANGLE_2_INTERSECTION_H

#include <CGAL/Iso_rectangle_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/utils.h>
#include <CGAL/number_utils.h>
#include <CGAL/Object.h>

CGAL_BEGIN_NAMESPACE
namespace CGALi {

template <class K>
class Segment_2_Iso_rectangle_2_pair {
public:
    enum Intersection_results {NO, POINT, SEGMENT};
    Segment_2_Iso_rectangle_2_pair() ;
    Segment_2_Iso_rectangle_2_pair(typename K::Segment_2 const *seg,
                          typename K::Iso_rectangle_2 const *rect) ;

#ifndef CGAL_CFG_RETURN_TYPE_BUG_2
  Intersection_results intersection_type() const;

#else
  Intersection_results intersection_type() const
 {
     typedef typename K::RT RT;
     typedef typename K::FT FT;
     if (_known)
         return _result;
     _known = true;
     for (int i=0; i<_ref_point.dimension(); i++) {
         if (_dir.homogeneous(i) == RT(0)) {
             if (_ref_point.cartesian(i) < _isomin.cartesian(i)) {
                 _result = NO;
                 return _result;
             }
             if (_ref_point.cartesian(i) > _isomax.cartesian(i)) {
                 _result = NO;
                 return _result;
             }
         } else {
             FT newmin, newmax;
             if (_dir.homogeneous(i) > RT(0)) {
                 newmin = (_isomin.cartesian(i)-_ref_point.cartesian(i)) /
                     _dir.cartesian(i);
                 newmax = (_isomax.cartesian(i)-_ref_point.cartesian(i)) /
                     _dir.cartesian(i);
             } else {
                 newmin = (_isomax.cartesian(i)-_ref_point.cartesian(i)) /
                     _dir.cartesian(i);
                 newmax = (_isomin.cartesian(i)-_ref_point.cartesian(i)) /
                     _dir.cartesian(i);
             }
             if (newmin > _min)
                 _min = newmin;
             if (newmax < _max)
                 _max = newmax;
             if (_max < _min) {
                 _result = NO;
                 return _result;
             }
         }
     }
     if (_max == _min) {
         _result = POINT;
         return _result;
     }
     _result = SEGMENT;
     return _result;
 }
 
#endif // CGAL_CFG_RETURN_TYPE_BUG_2

    bool                       intersection(
                                    typename K::Point_2 &result) const;
    bool                       intersection(
                                    typename K::Segment_2 &result) const;
protected:
    mutable bool                       _known;
    mutable Intersection_results       _result;
    mutable typename K::Point_2            _ref_point;
    mutable typename K::Vector_2           _dir;
    mutable typename K::Point_2            _isomin;
    mutable typename K::Point_2            _isomax;
    mutable typename K::FT              _min,
                               _max;
};

template <class K>
inline bool do_intersect(
    const typename CGAL_WRAP(K)::Segment_2 &p1,
    const typename CGAL_WRAP(K)::Iso_rectangle_2 &p2,
    const K&)
{
    typedef Segment_2_Iso_rectangle_2_pair<K> pair_t;
    pair_t pair(&p1, &p2);
    return pair.intersection_type() != pair_t::NO;
}





template <class K>
Object
intersection(
    const typename CGAL_WRAP(K)::Segment_2 &seg,
    const typename CGAL_WRAP(K)::Iso_rectangle_2 &iso,
    const K&)
{
    typedef Segment_2_Iso_rectangle_2_pair<K> is_t;
    is_t ispair(&seg, &iso);
    switch (ispair.intersection_type()) {
    case is_t::NO:
    default:
        return Object();
    case is_t::POINT: {
        typename K::Point_2 ipt;
        ispair.intersection(ipt);
        return make_object(ipt);
    }
    case is_t::SEGMENT: {
        typename K::Segment_2 iseg;
        ispair.intersection(iseg);
        return make_object(iseg);
    }
    }
}

template <class K>
inline
Object
intersection(const typename CGAL_WRAP(K)::Iso_rectangle_2 &iso,
	     const typename CGAL_WRAP(K)::Segment_2 &seg,
	     const K&)
{
  return CGALi::intersection(seg, iso, k);
}

template <class K>
Segment_2_Iso_rectangle_2_pair<K>::Segment_2_Iso_rectangle_2_pair()
{
    _known = false;
}

template <class K>
Segment_2_Iso_rectangle_2_pair<K>::
Segment_2_Iso_rectangle_2_pair(
        typename K::Segment_2 const *seg,
        typename K::Iso_rectangle_2 const *iso)
{
    _known = false;
    _isomin = iso->min();
    _isomax = iso->max();
    _ref_point = seg->source();
    _dir = seg->direction().to_vector();
    _min = (typename K::FT)(0);
    int main_dir = (CGAL_NTS abs(_dir.x()) > CGAL_NTS abs(_dir.y()) ) ? 0 : 1;
    _max = (seg->target().cartesian(main_dir)-_ref_point.cartesian(main_dir)) /
            _dir.cartesian(main_dir);
}

#ifndef CGAL_CFG_RETURN_TYPE_BUG_2
template <class K>
typename Segment_2_Iso_rectangle_2_pair<K>::Intersection_results
Segment_2_Iso_rectangle_2_pair<K>::intersection_type() const
{
    typedef typename K::RT RT;
    typedef typename K::FT FT;
    if (_known)
        return _result;
    _known = true;
    for (int i=0; i<_ref_point.dimension(); i++) {
        if (_dir.homogeneous(i) == RT(0)) {
            if (_ref_point.cartesian(i) < _isomin.cartesian(i)) {
                _result = NO;
                return _result;
            }
            if (_ref_point.cartesian(i) > _isomax.cartesian(i)) {
                _result = NO;
                return _result;
            }
        } else {
            FT newmin, newmax;
            if (_dir.homogeneous(i) > RT(0)) {
                newmin = (_isomin.cartesian(i)-_ref_point.cartesian(i)) /
                    _dir.cartesian(i);
                newmax = (_isomax.cartesian(i)-_ref_point.cartesian(i)) /
                    _dir.cartesian(i);
            } else {
                newmin = (_isomax.cartesian(i)-_ref_point.cartesian(i)) /
                    _dir.cartesian(i);
                newmax = (_isomin.cartesian(i)-_ref_point.cartesian(i)) /
                    _dir.cartesian(i);
            }
            if (newmin > _min)
                _min = newmin;
            if (newmax < _max)
                _max = newmax;
            if (_max < _min) {
                _result = NO;
                return _result;
            }
        }
    }
    if (_max == _min) {
        _result = POINT;
        return _result;
    }
    _result = SEGMENT;
    return _result;
}

#endif // CGAL_CFG_RETURN_TYPE_BUG_2

template <class K>
bool Segment_2_Iso_rectangle_2_pair<K>::
intersection(typename K::Segment_2 &seg) const
{
  typedef typename K::Segment_2 Segment_2;
    if (!_known)
        intersection_type();
    if (_result != SEGMENT)
        return false;
    typename K::Point_2 p1(_ref_point + _dir*_min);
    typename K::Point_2 p2(_ref_point + _dir*_max);
    seg = Segment_2(p1, p2);
    return true;
}

template <class K> bool Segment_2_Iso_rectangle_2_pair<K>::
intersection(typename K::Point_2 &pt) const
{
  typedef typename K::Point_2 Point_2;
    if (!_known)
        intersection_type();
    if (_result != POINT)
        return false;
    pt = Point_2(_ref_point + _dir*_min);
    return true;
}



template <class K>
class Iso_rectangle_2_Segment_2_pair:
          public Segment_2_Iso_rectangle_2_pair<K> {
public:
    Iso_rectangle_2_Segment_2_pair() {}
    Iso_rectangle_2_Segment_2_pair(typename K::Iso_rectangle_2 const *rect,
                               typename K::Segment_2 const *seg)
                :Segment_2_Iso_rectangle_2_pair<K> (seg, rect){}
};

template <class K>
inline bool do_intersect(
    const typename CGAL_WRAP(K)::Iso_rectangle_2 &p1,
    const typename CGAL_WRAP(K)::Segment_2 &p2,
    const K&)
{
    typedef Iso_rectangle_2_Segment_2_pair<K> pair_t;
    pair_t pair(&p1, &p2);
    return pair.intersection_type() != pair_t::NO;
}

} // namespace CGALi

template <class K>
inline bool
do_intersect(const Iso_rectangle_2<K> & iso,
	     const Segment_2<K> &seg)
{
  return CGALi::do_intersect(seg, iso, K());
}

template <class K>
inline bool
do_intersect(const Segment_2<K> &seg, const Iso_rectangle_2<K> &iso)
{
  return CGALi::do_intersect(seg, iso, K());
}


template <class K>
inline Object
intersection(
    const Iso_rectangle_2<K> &iso,
    const Segment_2<K> &seg)
{
    return CGALi::intersection(seg, iso, K());
}

template <class K>
inline Object
intersection(const Segment_2<K> &seg,
	       const Iso_rectangle_2<K> &iso)
{
    return CGALi::intersection(seg, iso, K());
}

CGAL_END_NAMESPACE

#endif // CGAL_SEGMENT_2_ISO_RECTANGLE_2_INTERSECTION_H
