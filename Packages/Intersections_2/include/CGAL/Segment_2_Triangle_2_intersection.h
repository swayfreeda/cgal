
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
// file          : include/CGAL/Segment_2_Triangle_2_intersection.h
// source        : intersection_2_2.fw
// author(s)     : Geert-Jan Giezeman
//
// coordinator   : Saarbruecken
//
// ============================================================================


#ifndef CGAL_SEGMENT_2_TRIANGLE_2_INTERSECTION_H
#define CGAL_SEGMENT_2_TRIANGLE_2_INTERSECTION_H

#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/utils.h>
#include <CGAL/number_utils.h>
#include <CGAL/Straight_2.h>
#include <CGAL/Object.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

template <class K>
class Segment_2_Triangle_2_pair {
public:
    enum Intersection_results {NO, POINT, SEGMENT};
    Segment_2_Triangle_2_pair() ;
    Segment_2_Triangle_2_pair(typename K::Segment_2 const *seg,
                            typename K::Triangle_2 const *trian);
    ~Segment_2_Triangle_2_pair() {}
#ifdef CGAL_CFG_RETURN_TYPE_BUG_2
    Intersection_results intersection_type() const
    {
        if (_known)
            return _result;
    // The non const this pointer is used to cast away const.
        _known = true;
        Straight_2_<K> straight(*_seg);
    Line_2<K> l(_trian->vertex(0), _trian->vertex(1));
    if (l.oriented_side(_trian->vertex(2)) == ON_POSITIVE_SIDE) {
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(0), _trian->vertex(1)));
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(1), _trian->vertex(2)));
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(2), _trian->vertex(0)));
        } else {
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(2), _trian->vertex(1)));
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(1), _trian->vertex(0)));
            straight.cut_right_off(
                Line_2<K>(_trian->vertex(0), _trian->vertex(2)));
        }
        switch (straight.current_state()) {
        case Straight_2_<K>::EMPTY:
            _result = NO;
            return _result;
        case Straight_2_<K>::POINT: {
            straight.current(_intersection_point);
            _result = POINT;
            return _result;
            }
        case Straight_2_<K>::SEGMENT: {
            typename K::Segment_2 seg;
            straight.current(seg);
            _intersection_point = seg.start();
            _other_point = seg.end();
            _result = SEGMENT;
            return _result;
            }
        default:  // should not happen.
            CGAL_kernel_assertion_msg(false, "Internal CGAL error.");
            _result = NO;
            return _result;
        }
    }
    
#else
    Intersection_results intersection_type() const;
#endif // CGAL_CFG_RETURN_TYPE_BUG_2
    bool                intersection(typename K::Point_2 &result) const;
    bool                intersection(typename K::Segment_2 &result) const;
protected:
    typename K::Segment_2 const *  _seg;
    typename K::Triangle_2 const * _trian;
    mutable bool                       _known;
    mutable Intersection_results       _result;
    mutable typename K::Point_2            _intersection_point;
    mutable typename K::Point_2            _other_point;
};

template <class K>
inline bool do_intersect(
    const typename CGAL_WRAP(K)::Segment_2 &p1,
    const typename CGAL_WRAP(K)::Triangle_2 &p2,
    const K&)
{
    typedef Segment_2_Triangle_2_pair<K> pair_t;
    pair_t pair(&p1, &p2);
    return pair.intersection_type() != pair_t::NO;
}





template <class K>
Segment_2_Triangle_2_pair<K>::
Segment_2_Triangle_2_pair()
{
    _known = false;
    _seg = 0;
    _trian = 0;
}

template <class K>
Segment_2_Triangle_2_pair<K>::
Segment_2_Triangle_2_pair(typename K::Segment_2 const *seg,
                            typename K::Triangle_2 const *trian)
{
    _known = false;
    _seg = seg;
    _trian = trian;
}

#ifndef CGAL_CFG_RETURN_TYPE_BUG_2
template <class K>
typename Segment_2_Triangle_2_pair<K>::Intersection_results
Segment_2_Triangle_2_pair<K>::intersection_type() const
{
    if (_known)
        return _result;
// The non const this pointer is used to cast away const.
    _known = true;
    Straight_2_<K> straight(*_seg);
Line_2<K> l(_trian->vertex(0), _trian->vertex(1));
if (l.oriented_side(_trian->vertex(2)) == ON_POSITIVE_SIDE) {
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(0), _trian->vertex(1)));
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(1), _trian->vertex(2)));
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(2), _trian->vertex(0)));
    } else {
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(2), _trian->vertex(1)));
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(1), _trian->vertex(0)));
        straight.cut_right_off(
            Line_2<K>(_trian->vertex(0), _trian->vertex(2)));
    }
    switch (straight.current_state()) {
    case Straight_2_<K>::EMPTY:
        _result = NO;
        return _result;
    case Straight_2_<K>::POINT: {
        straight.current(_intersection_point);
        _result = POINT;
        return _result;
        }
    case Straight_2_<K>::SEGMENT: {
        typename K::Segment_2 seg;
        straight.current(seg);
        _intersection_point = seg.start();
        _other_point = seg.end();
        _result = SEGMENT;
        return _result;
        }
    default:  // should not happen.
        CGAL_kernel_assertion_msg(false, "Internal CGAL error.");
        _result = NO;
        return _result;
    }
}

#endif // CGAL_CFG_RETURN_TYPE_BUG_2

template <class K>
bool
Segment_2_Triangle_2_pair<K>::
intersection(typename K::Point_2 &result) const
{
    if (!_known)
        intersection_type();
    if (_result != POINT)
        return false;
    result = _intersection_point;
    return true;
}

template <class K>
bool
Segment_2_Triangle_2_pair<K>::
intersection(typename K::Segment_2 &result) const
{
  typedef typename K::Segment_2 Segment_2; 
    if (!_known)
        intersection_type();
    if (_result != SEGMENT)
        return false;
    result = Segment_2(_intersection_point, _other_point);
    return true;
}




template <class K>
Object
intersection(const typename CGAL_WRAP(K)::Segment_2 &seg, 
	     const typename CGAL_WRAP(K)::Triangle_2&tr,
	     const K&)
{
    typedef Segment_2_Triangle_2_pair<K> is_t;
    is_t ispair(&seg, &tr);
    switch (ispair.intersection_type()) {
    case is_t::NO:
    default:
        return Object();
    case is_t::POINT: {
        typename K::Point_2 pt;
        ispair.intersection(pt);
        return make_object(pt);
    }
    case is_t::SEGMENT: {
        typename K::Segment_2 iseg;
        ispair.intersection(iseg);
        return make_object(iseg);
    }
    }
}


template <class K>
Object
intersection(const typename CGAL_WRAP(K)::Triangle_2&tr,
	     const typename CGAL_WRAP(K)::Segment_2 &seg, 
	     const K& k)
{
  return CGALi::intersection(seg, tr, k);
}


template <class K>
class Triangle_2_Segment_2_pair
: public Segment_2_Triangle_2_pair<K> {
public:
    Triangle_2_Segment_2_pair(
            typename K::Triangle_2 const *trian,
            typename K::Segment_2 const *seg) :
                        Segment_2_Triangle_2_pair<K>(seg, trian) {}
};

template <class K>
inline bool do_intersect(
    const typename CGAL_WRAP(K)::Triangle_2 &p1,
    const typename CGAL_WRAP(K)::Segment_2 &p2,
    const K&)
{
    typedef Triangle_2_Segment_2_pair<K> pair_t;
    pair_t pair(&p1, &p2);
    return pair.intersection_type() != pair_t::NO;
}

} // namespace CGALi

template <class K>
inline Object
intersection(const Segment_2<K> &seg, const Triangle_2<K> &tr)
{
    return CGALi::intersection(seg, tr, K());
}

template <class K>
inline Object
intersection(const Triangle_2<K> &tr, const Segment_2<K> &seg)
{
    return CGALi::intersection(seg, tr, K());
}

template <class K>
inline bool
do_intersect(const Segment_2<K> &seg, const Triangle_2<K> &tr)
{
    return CGALi::do_intersect(seg, tr, K());
}

template <class K>
inline bool
do_intersect(const Triangle_2<K> &tr, const Segment_2<K> &seg)
{
    return CGALi::do_intersect(seg, tr, K());
}

CGAL_END_NAMESPACE

#endif
