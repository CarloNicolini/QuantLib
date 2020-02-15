/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004 StatPro Italia srl
 Copyright (C) 2009 Ferdinando Ametrano
 Copyright (C) 2019 SoftSolutions! S.r.l.

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file zeroyieldstructure.hpp
    \brief Zero-yield based term structure
*/

#ifndef quantlib_zero_yield_structure_simple_hpp
#define quantlib_zero_yield_structure_simple_hpp

#include <ql/termstructures/yieldtermstructure.hpp>

namespace QuantLib {

//! Zero-yield term structure
/*! This abstract class acts as an adapter to YieldTermStructure
    allowing the programmer to implement only the
    <tt>simpleZeroYieldImpl(Time)</tt> method in derived classes.

    Discount and forward are calculated from simple zero yields.

    Zero rates are assumed to be annual simple compounding.

    \ingroup yieldtermstructures
*/
class ZeroYieldStructureSimple : public YieldTermStructure {
  public:
    /*! \name Constructors
        See the TermStructure documentation for issues regarding
        constructors.
    */
    //@{
    ZeroYieldStructureSimple(const DayCounter &dc = DayCounter(),
                             const std::vector<Handle<Quote> > &jumps = std::vector<Handle<Quote> >(),
                             const std::vector<Date> &jumpDates = std::vector<Date>());
    ZeroYieldStructureSimple(const Date &referenceDate, const Calendar &calendar = Calendar(),
                             const DayCounter &dc = DayCounter(),
                             const std::vector<Handle<Quote> > &jumps = std::vector<Handle<Quote> >(),
                             const std::vector<Date> &jumpDates = std::vector<Date>());
    ZeroYieldStructureSimple(Natural settlementDays, const Calendar &calendar, const DayCounter &dc = DayCounter(),
                             const std::vector<Handle<Quote> > &jumps = std::vector<Handle<Quote> >(),
                             const std::vector<Date> &jumpDates = std::vector<Date>());
    //@}
  protected:
    /*! \name Calculations

        This method must be implemented in derived classes to
        perform the actual calculations. When it is called,
        range check has already been performed; therefore, it
        must assume that extrapolation is required.
    */
    //@{
    //! zero-yield calculation
    virtual Rate simpleZeroYieldImpl(Time) const = 0;
    virtual Rate simpleZeroYieldImpl(const Date&) const;
    //@}

    //! \name YieldTermStructure implementation
    //@{
    /*! Returns the discount factor for the given date calculating it
        from the zero yield.
    */
    DiscountFactor discountImpl(Time) const;
    DiscountFactor discountImpl(const Date&) const;
    //@}
};

// inline definitions

inline Rate ZeroYieldStructureSimple::simpleZeroYieldImpl(const Date& d) const {
    return simpleZeroYieldImpl(timeFromReference(d));
}

inline DiscountFactor ZeroYieldStructureSimple::discountImpl(Time t) const {
    if (t == 0.0)   // this acts as a safe guard in cases where
        return 1.0; // zeroYieldImpl(0.0) would throw.

    Rate r = simpleZeroYieldImpl(t);
	return DiscountFactor(1.0 / (1.0 + r * t));
}

} // namespace QuantLib

#endif
