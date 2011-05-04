/***************************************************************************
 *  include/stxxl/bits/stream/sort_stream.h
 *
 *  Part of the STXXL. See http://stxxl.sourceforge.net
 *
 *  Copyright (C) 2002-2005 Roman Dementiev <dementiev@mpi-sb.mpg.de>
 *  Copyright (C) 2006-2008 Johannes Singler <singler@ira.uka.de>
 *  Copyright (C) 2008-2010 Andreas Beckmann <beckmann@cs.uni-frankfurt.de>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

#ifndef STXXL_SORT_STREAM_HEADER
#define STXXL_SORT_STREAM_HEADER

#ifdef STXXL_BOOST_CONFIG
 #include <boost/config.hpp>
#endif

#include <stxxl/bits/stream/stream.h>
#include <stxxl/bits/mng/mng.h>
#include <stxxl/bits/algo/sort_base.h>
#include <stxxl/bits/algo/sort_helper.h>
#include <stxxl/bits/algo/adaptor.h>
#include <stxxl/bits/algo/run_cursor.h>
#include <stxxl/bits/algo/losertree.h>
#include <stxxl/bits/stream/sorted_runs.h>


__STXXL_BEGIN_NAMESPACE

namespace stream
{
    //! \addtogroup streampack Stream package
    //! \{


    ////////////////////////////////////////////////////////////////////////
    //     CREATE RUNS                                                    //
    ////////////////////////////////////////////////////////////////////////

    //! \brief Forms sorted runs of data from a stream
    //!
    //! \tparam Input_ type of the input stream
    //! \tparam Cmp_ type of comparison object used for sorting the runs
    //! \tparam BlockSize_ size of blocks used to store the runs (in bytes)
    //! \tparam AllocStr_ functor that defines allocation strategy for the runs
    template <
        class Input_,
        class Cmp_,
        unsigned BlockSize_ = STXXL_DEFAULT_BLOCK_SIZE(typename Input_::value_type),
        class AllocStr_ = STXXL_DEFAULT_ALLOC_STRATEGY>
    class basic_runs_creator : private noncopyable
    {
    protected:
        Input_ & input;
        Cmp_ cmp;

    public:
        typedef Cmp_ cmp_type;
        typedef typename Input_::value_type value_type;
        typedef typed_block<BlockSize_, value_type> block_type;
        typedef sort_helper::trigger_entry<block_type> trigger_entry_type;
        typedef sorted_runs<trigger_entry_type> sorted_runs_type;

    private:
        typedef typename sorted_runs_type::run_type run_type;
        sorted_runs_type result_; // stores the result (sorted runs)
        unsigned_type m_;         // memory for internal use in blocks
        bool result_computed;     // true iff result is already computed (used in 'result' method)

        //! \brief Fetch data from input into blocks[first_idx,last_idx).
        unsigned_type fetch(block_type * blocks, unsigned_type first_idx, unsigned_type last_idx)
        {
            typename element_iterator_traits<block_type>::element_iterator output =
                make_element_iterator(blocks, first_idx);
            unsigned_type curr_idx = first_idx;
            while (!input.empty() && curr_idx != last_idx) {
                *output = *input;
                ++input;
                ++output;
                ++curr_idx;
            }
            return curr_idx;
        }

        void fill_with_max_value(block_type * blocks, unsigned_type num_blocks, unsigned_type first_idx)
        {
            unsigned_type last_idx = num_blocks * block_type::size;
            if (first_idx < last_idx) {
                typename element_iterator_traits<block_type>::element_iterator curr =
                    make_element_iterator(blocks, first_idx);
                while (first_idx != last_idx) {
                    *curr = cmp.max_value();
                    ++curr;
                    ++first_idx;
                }
            }
        }

        //! \brief Sort a specific run, contained in a sequences of blocks.
        void sort_run(block_type * run, unsigned_type elements)
        {
            potentially_parallel::
            sort(make_element_iterator(run, 0),
                 make_element_iterator(run, elements),
                 cmp);
        }

        void compute_result();

    public:
        //! \brief Create the object
        //! \param i input stream
        //! \param c comparator object
        //! \param memory_to_use memory amount that is allowed to used by the sorter in bytes
        basic_runs_creator(Input_ & i, Cmp_ c, unsigned_type memory_to_use) :
            input(i), cmp(c), m_(memory_to_use / BlockSize_ / sort_memory_usage_factor()), result_computed(false)
        {
            sort_helper::verify_sentinel_strict_weak_ordering(cmp);
            if (!(2 * BlockSize_ * sort_memory_usage_factor() <= memory_to_use)) {
                throw bad_parameter("stxxl::runs_creator<>:runs_creator(): INSUFFICIENT MEMORY provided, please increase parameter 'memory_to_use'");
            }
            assert(m_ > 0);
        }

        //! \brief Returns the sorted runs object
        //! \return Sorted runs object. The result is computed lazily, i.e. on the first call
        //! \remark Returned object is intended to be used by \c runs_merger object as input
        const sorted_runs_type & result()
        {
            if (!result_computed)
            {
                compute_result();
                result_computed = true;
#ifdef STXXL_PRINT_STAT_AFTER_RF
                STXXL_MSG(*stats::get_instance());
#endif //STXXL_PRINT_STAT_AFTER_RF
            }
            return result_;
        }
    };

    //! \brief Finish the results, i. e. create all runs.
    //!
    //! This is the main routine of this class.
    template <class Input_, class Cmp_, unsigned BlockSize_, class AllocStr_>
    void basic_runs_creator<Input_, Cmp_, BlockSize_, AllocStr_>::compute_result()
    {
        unsigned_type i = 0;
        unsigned_type m2 = m_ / 2;
        const unsigned_type el_in_run = m2 * block_type::size; // # el in a run
        STXXL_VERBOSE1("basic_runs_creator::compute_result m2=" << m2);
        unsigned_type blocks1_length = 0, blocks2_length = 0;
        block_type * Blocks1 = NULL;

#ifndef STXXL_SMALL_INPUT_PSORT_OPT
        Blocks1 = new block_type[m2 * 2];
#else
        while (!input.empty() && blocks1_length != block_type::size)
        {
            result_.small_.push_back(*input);
            ++input;
            ++blocks1_length;
        }

        if (blocks1_length == block_type::size && !input.empty())
        {
            Blocks1 = new block_type[m2 * 2];
            std::copy(result_.small_.begin(), result_.small_.end(), Blocks1[0].begin());
            result_.small_.clear();
        }
        else
        {
            STXXL_VERBOSE1("basic_runs_creator: Small input optimization, input length: " << blocks1_length);
            result_.elements = blocks1_length;
            potentially_parallel::
            sort(result_.small_.begin(), result_.small_.end(), cmp);
            return;
        }
#endif //STXXL_SMALL_INPUT_PSORT_OPT

        // the first block may be there already
        blocks1_length = fetch(Blocks1, blocks1_length, el_in_run);

        // sort first run
        sort_run(Blocks1, blocks1_length);
        if (blocks1_length <= block_type::size && input.empty())
        {
            // small input, do not flush it on the disk(s)
            STXXL_VERBOSE1("basic_runs_creator: Small input optimization, input length: " << blocks1_length);
            assert(result_.small_.empty());
            result_.small_.insert(result_.small_.end(), Blocks1[0].begin(), Blocks1[0].begin() + blocks1_length);
            result_.elements = blocks1_length;
            delete[] Blocks1;
            return;
        }

        block_type * Blocks2 = Blocks1 + m2;
        block_manager * bm = block_manager::get_instance();
        request_ptr * write_reqs = new request_ptr[m2];
        run_type run;

        unsigned_type cur_run_size = div_ceil(blocks1_length, block_type::size);  // in blocks
        run.resize(cur_run_size);
        bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

        disk_queues::get_instance()->set_priority_op(request_queue::WRITE);

        // fill the rest of the last block with max values
        fill_with_max_value(Blocks1, cur_run_size, blocks1_length);

        for (i = 0; i < cur_run_size; ++i)
        {
            run[i].value = Blocks1[i][0];
            write_reqs[i] = Blocks1[i].write(run[i].bid);
        }
        result_.runs.push_back(run);
        result_.runs_sizes.push_back(blocks1_length);
        result_.elements += blocks1_length;

        if (input.empty())
        {
            // return
            wait_all(write_reqs, write_reqs + cur_run_size);
            delete[] write_reqs;
            delete[] Blocks1;
            return;
        }

        STXXL_VERBOSE1("Filling the second part of the allocated blocks");
        blocks2_length = fetch(Blocks2, 0, el_in_run);

        if (input.empty())
        {
            // optimization if the whole set fits into both halves
            // (re)sort internally and return
            blocks2_length += el_in_run;
            sort_run(Blocks1, blocks2_length);  // sort first an second run together
            wait_all(write_reqs, write_reqs + cur_run_size);
            bm->delete_blocks(make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

            cur_run_size = div_ceil(blocks2_length, block_type::size);
            run.resize(cur_run_size);
            bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

            // fill the rest of the last block with max values
            fill_with_max_value(Blocks1, cur_run_size, blocks2_length);

            assert(cur_run_size > m2);

            for (i = 0; i < m2; ++i)
            {
                run[i].value = Blocks1[i][0];
                write_reqs[i]->wait();
                write_reqs[i] = Blocks1[i].write(run[i].bid);
            }

            request_ptr * write_reqs1 = new request_ptr[cur_run_size - m2];

            for ( ; i < cur_run_size; ++i)
            {
                run[i].value = Blocks1[i][0];
                write_reqs1[i - m2] = Blocks1[i].write(run[i].bid);
            }

            result_.runs[0] = run;
            result_.runs_sizes[0] = blocks2_length;
            result_.elements = blocks2_length;

            wait_all(write_reqs, write_reqs + m2);
            delete[] write_reqs;
            wait_all(write_reqs1, write_reqs1 + cur_run_size - m2);
            delete[] write_reqs1;

            delete[] Blocks1;

            return;
        }

        // more than 2 runs can be filled, i. e. the general case

        sort_run(Blocks2, blocks2_length);

        cur_run_size = div_ceil(blocks2_length, block_type::size);  // in blocks
        run.resize(cur_run_size);
        bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

        for (i = 0; i < cur_run_size; ++i)
        {
            run[i].value = Blocks2[i][0];
            write_reqs[i]->wait();
            write_reqs[i] = Blocks2[i].write(run[i].bid);
        }
        assert((blocks2_length % el_in_run) == 0);

        result_.runs.push_back(run);
        result_.runs_sizes.push_back(blocks2_length);
        result_.elements += blocks2_length;

        while (!input.empty())
        {
            blocks1_length = fetch(Blocks1, 0, el_in_run);
            sort_run(Blocks1, blocks1_length);
            cur_run_size = div_ceil(blocks1_length, block_type::size);  // in blocks
            run.resize(cur_run_size);
            bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

            // fill the rest of the last block with max values (occurs only on the last run)
            fill_with_max_value(Blocks1, cur_run_size, blocks1_length);

            for (i = 0; i < cur_run_size; ++i)
            {
                run[i].value = Blocks1[i][0];
                write_reqs[i]->wait();
                write_reqs[i] = Blocks1[i].write(run[i].bid);
            }
            result_.runs.push_back(run);
            result_.runs_sizes.push_back(blocks1_length);
            result_.elements += blocks1_length;

            std::swap(Blocks1, Blocks2);
            std::swap(blocks1_length, blocks2_length);
        }

        wait_all(write_reqs, write_reqs + m2);
        delete[] write_reqs;
        delete[] ((Blocks1 < Blocks2) ? Blocks1 : Blocks2);
    }

    //! \brief Forms sorted runs of data from a stream
    //!
    //! \tparam Input_ type of the input stream
    //! \tparam Cmp_ type of omparison object used for sorting the runs
    //! \tparam BlockSize_ size of blocks used to store the runs
    //! \tparam AllocStr_ functor that defines allocation strategy for the runs
    template <
        class Input_,
        class Cmp_,
        unsigned BlockSize_ = STXXL_DEFAULT_BLOCK_SIZE(typename Input_::value_type),
        class AllocStr_ = STXXL_DEFAULT_ALLOC_STRATEGY>
    class runs_creator : public basic_runs_creator<Input_, Cmp_, BlockSize_, AllocStr_>
    {
    private:
        typedef basic_runs_creator<Input_, Cmp_, BlockSize_, AllocStr_> base;

    public:
        typedef typename base::block_type block_type;

    private:
        using base::input;

    public:
        //! \brief Creates the object
        //! \param i input stream
        //! \param c comparator object
        //! \param memory_to_use memory amount that is allowed to used by the sorter in bytes
        runs_creator(Input_ & i, Cmp_ c, unsigned_type memory_to_use) : base(i, c, memory_to_use)
        { }
    };


    //! \brief Input strategy for \c runs_creator class
    //!
    //! This strategy together with \c runs_creator class
    //! allows to create sorted runs
    //! data structure usable for \c runs_merger
    //! pushing elements into the sorter
    //! (using runs_creator::push())
    template <class ValueType_>
    struct use_push
    {
        typedef ValueType_ value_type;
    };

    //! \brief Forms sorted runs of elements passed in push() method
    //!
    //! A specialization of \c runs_creator that
    //! allows to create sorted runs
    //! data structure usable for \c runs_merger from
    //! elements passed in sorted push() method. <BR>
    //! \tparam ValueType_ type of values (parameter for \c use_push strategy)
    //! \tparam Cmp_ type of comparison object used for sorting the runs
    //! \tparam BlockSize_ size of blocks used to store the runs
    //! \tparam AllocStr_ functor that defines allocation strategy for the runs
    template <
        class ValueType_,
        class Cmp_,
        unsigned BlockSize_,
        class AllocStr_>
    class runs_creator<
        use_push<ValueType_>,
        Cmp_,
        BlockSize_,
        AllocStr_>
        : private noncopyable
    {
        Cmp_ cmp;

    public:
        typedef Cmp_ cmp_type;
        typedef ValueType_ value_type;
        typedef typed_block<BlockSize_, value_type> block_type;
        typedef sort_helper::trigger_entry<block_type> trigger_entry_type;
        typedef sorted_runs<trigger_entry_type> sorted_runs_type;
        typedef sorted_runs_type result_type;

    private:
        typedef typename sorted_runs_type::run_type run_type;
        sorted_runs_type result_; // stores the result (sorted runs)
        unsigned_type m_;         // memory for internal use in blocks

        bool result_computed;     // true after the result() method was called for the first time

        const unsigned_type m2;
        const unsigned_type el_in_run;
        unsigned_type cur_el;
        block_type * Blocks1;
        block_type * Blocks2;
        request_ptr * write_reqs;
        run_type run;

        void fill_with_max_value(block_type * blocks, unsigned_type num_blocks, unsigned_type first_idx)
        {
            unsigned_type last_idx = num_blocks * block_type::size;
            if (first_idx < last_idx) {
                typename element_iterator_traits<block_type>::element_iterator curr =
                    make_element_iterator(blocks, first_idx);
                while (first_idx != last_idx) {
                    *curr = cmp.max_value();
                    ++curr;
                    ++first_idx;
                }
            }
        }

        void sort_run(block_type * run, unsigned_type elements)
        {
            potentially_parallel::
            sort(make_element_iterator(run, 0),
                 make_element_iterator(run, elements),
                 cmp);
        }

        void compute_result()
        {
            if (cur_el == 0)
                return;

            unsigned_type cur_el_reg = cur_el;
            sort_run(Blocks1, cur_el_reg);
            result_.elements += cur_el_reg;
            if (cur_el_reg <= block_type::size && result_.elements == cur_el_reg)
            {
                // small input, do not flush it on the disk(s)
                STXXL_VERBOSE1("runs_creator(use_push): Small input optimization, input length: " << cur_el_reg);
                result_.small_.resize(cur_el_reg);
                std::copy(Blocks1[0].begin(), Blocks1[0].begin() + cur_el_reg, result_.small_.begin());
                return;
            }

            const unsigned_type cur_run_size = div_ceil(cur_el_reg, block_type::size);     // in blocks
            run.resize(cur_run_size);
            block_manager * bm = block_manager::get_instance();
            bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

            disk_queues::get_instance()->set_priority_op(request_queue::WRITE);

            result_.runs_sizes.push_back(cur_el_reg);

            // fill the rest of the last block with max values
            fill_with_max_value(Blocks1, cur_run_size, cur_el_reg);

            unsigned_type i = 0;
            for ( ; i < cur_run_size; ++i)
            {
                run[i].value = Blocks1[i][0];
                if (write_reqs[i].get())
                    write_reqs[i]->wait();

                write_reqs[i] = Blocks1[i].write(run[i].bid);
            }
            result_.runs.push_back(run);

            for (i = 0; i < m2; ++i)
                if (write_reqs[i].get())
                    write_reqs[i]->wait();
        }

        void cleanup()
        {
            delete[] write_reqs;
            delete[] ((Blocks1 < Blocks2) ? Blocks1 : Blocks2);
            write_reqs = NULL;
            Blocks1 = Blocks2 = NULL;
        }

    public:
        //! \brief Creates the object
        //! \param c comparator object
        //! \param memory_to_use memory amount that is allowed to used by the sorter in bytes
        runs_creator(Cmp_ c, unsigned_type memory_to_use) :
            cmp(c), m_(memory_to_use / BlockSize_ / sort_memory_usage_factor()), result_computed(false),
            m2(m_ / 2),
            el_in_run(m2 * block_type::size),
            cur_el(0),
            Blocks1(new block_type[m2 * 2]),
            Blocks2(Blocks1 + m2),
            write_reqs(new request_ptr[m2])
        {
            sort_helper::verify_sentinel_strict_weak_ordering(cmp);
            if (!(2 * BlockSize_ * sort_memory_usage_factor() <= memory_to_use)) {
                throw bad_parameter("stxxl::runs_creator<>:runs_creator(): INSUFFICIENT MEMORY provided, please increase parameter 'memory_to_use'");
            }
            assert(m2 > 0);
        }

        ~runs_creator()
        {
            if (!result_computed)
                cleanup();
        }

        //! \brief Adds new element to the sorter
        //! \param val value to be added
        void push(const value_type & val)
        {
            assert(result_computed == false);
            unsigned_type cur_el_reg = cur_el;
            if (cur_el_reg < el_in_run)
            {
                Blocks1[cur_el_reg / block_type::size][cur_el_reg % block_type::size] = val;
                ++cur_el;
                return;
            }

            assert(el_in_run == cur_el);
            cur_el = 0;

            //sort and store Blocks1
            sort_run(Blocks1, el_in_run);
            result_.elements += el_in_run;

            const unsigned_type cur_run_size = div_ceil(el_in_run, block_type::size);    // in blocks
            run.resize(cur_run_size);
            block_manager * bm = block_manager::get_instance();
            bm->new_blocks(AllocStr_(), make_bid_iterator(run.begin()), make_bid_iterator(run.end()));

            disk_queues::get_instance()->set_priority_op(request_queue::WRITE);

            result_.runs_sizes.push_back(el_in_run);

            for (unsigned_type i = 0; i < cur_run_size; ++i)
            {
                run[i].value = Blocks1[i][0];
                if (write_reqs[i].get())
                    write_reqs[i]->wait();

                write_reqs[i] = Blocks1[i].write(run[i].bid);
            }

            result_.runs.push_back(run);

            std::swap(Blocks1, Blocks2);

            push(val);
        }

        //! \brief Returns the sorted runs object
        //! \return Sorted runs object.
        //! \remark Returned object is intended to be used by \c runs_merger object as input
        const sorted_runs_type & result()
        {
            if (1)
            {
                if (!result_computed)
                {
                    compute_result();
                    result_computed = true;
                    cleanup();
#ifdef STXXL_PRINT_STAT_AFTER_RF
                    STXXL_MSG(*stats::get_instance());
#endif //STXXL_PRINT_STAT_AFTER_RF
                }
            }
            return result_;
        }
    };


    //! \brief Input strategy for \c runs_creator class
    //!
    //! This strategy together with \c runs_creator class
    //! allows to create sorted runs
    //! data structure usable for \c runs_merger from
    //! sequences of elements in sorted order
    template <class ValueType_>
    struct from_sorted_sequences
    {
        typedef ValueType_ value_type;
    };

    //! \brief Forms sorted runs of data taking elements in sorted order (element by element)
    //!
    //! A specialization of \c runs_creator that
    //! allows to create sorted runs
    //! data structure usable for \c runs_merger from
    //! sequences of elements in sorted order. <BR>
    //! \tparam ValueType_ type of values (parameter for \c from_sorted_sequences strategy)
    //! \tparam Cmp_ type of comparison object used for sorting the runs
    //! \tparam BlockSize_ size of blocks used to store the runs
    //! \tparam AllocStr_ functor that defines allocation strategy for the runs
    template <
        class ValueType_,
        class Cmp_,
        unsigned BlockSize_,
        class AllocStr_>
    class runs_creator<
        from_sorted_sequences<ValueType_>,
        Cmp_,
        BlockSize_,
        AllocStr_>
        : private noncopyable
    {
        typedef ValueType_ value_type;
        typedef typed_block<BlockSize_, value_type> block_type;
        typedef sort_helper::trigger_entry<block_type> trigger_entry_type;
        typedef AllocStr_ alloc_strategy_type;
        Cmp_ cmp;

    public:
        typedef Cmp_ cmp_type;
        typedef sorted_runs<trigger_entry_type> sorted_runs_type;
        typedef sorted_runs_type result_type;

    private:
        typedef typename sorted_runs_type::run_type run_type;

        sorted_runs_type result_; // stores the result (sorted runs)
        unsigned_type m_;         // memory for internal use in blocks
        buffered_writer<block_type> writer;
        block_type * cur_block;
        unsigned_type offset;
        unsigned_type iblock;
        unsigned_type irun;
        alloc_strategy_type alloc_strategy;  // needs to be reset after each run

    public:
        //! \brief Creates the object
        //! \param c comparator object
        //! \param memory_to_use memory amount that is allowed to used by the sorter in bytes.
        //! Recommended value: 2 * block_size * D
        runs_creator(Cmp_ c, unsigned_type memory_to_use) :
            cmp(c),
            m_(memory_to_use / BlockSize_ / sort_memory_usage_factor()),
            writer(m_, m_ / 2),
            cur_block(writer.get_free_block()),
            offset(0),
            iblock(0),
            irun(0)
        {
            sort_helper::verify_sentinel_strict_weak_ordering(cmp);
            assert(m_ > 0);
            if (!(2 * BlockSize_ * sort_memory_usage_factor() <= memory_to_use)) {
                throw bad_parameter("stxxl::runs_creator<>:runs_creator(): INSUFFICIENT MEMORY provided, please increase parameter 'memory_to_use'");
            }
        }

        //! \brief Adds new element to the current run
        //! \param val value to be added to the current run
        void push(const value_type & val)
        {
            assert(offset < block_type::size);

            (*cur_block)[offset] = val;
            ++offset;

            if (offset == block_type::size)
            {
                // write current block

                block_manager * bm = block_manager::get_instance();
                // allocate space for the block
                result_.runs.resize(irun + 1);
                result_.runs[irun].resize(iblock + 1);
                bm->new_blocks(
                    alloc_strategy,
                    make_bid_iterator(result_.runs[irun].begin() + iblock),
                    make_bid_iterator(result_.runs[irun].end()),
                    iblock
                    );

                result_.runs[irun][iblock].value = (*cur_block)[0];         // init trigger
                cur_block = writer.write(cur_block, result_.runs[irun][iblock].bid);
                ++iblock;

                offset = 0;
            }

            ++result_.elements;
        }

        //! \brief Finishes current run and begins new one
        void finish()
        {
            if (offset == 0 && iblock == 0) // current run is empty
                return;


            result_.runs_sizes.resize(irun + 1);
            result_.runs_sizes.back() = iblock * block_type::size + offset;

            if (offset)    // if current block is partially filled
            {
                while (offset != block_type::size)
                {
                    (*cur_block)[offset] = cmp.max_value();
                    ++offset;
                }
                offset = 0;

                block_manager * bm = block_manager::get_instance();
                // allocate space for the block
                result_.runs.resize(irun + 1);
                result_.runs[irun].resize(iblock + 1);
                bm->new_blocks(
                    alloc_strategy,
                    make_bid_iterator(result_.runs[irun].begin() + iblock),
                    make_bid_iterator(result_.runs[irun].end()),
                    iblock
                    );

                result_.runs[irun][iblock].value = (*cur_block)[0];         // init trigger
                cur_block = writer.write(cur_block, result_.runs[irun][iblock].bid);
            }
            else
            { }

            alloc_strategy = alloc_strategy_type();  // reinitialize block allocator for the next run
            iblock = 0;
            ++irun;
        }

        //! \brief Returns the sorted runs object
        //! \return Sorted runs object
        //! \remark Returned object is intended to be used by \c runs_merger object as input
        const sorted_runs_type & result()
        {
            finish();
            writer.flush();

            return result_;
        }
    };


    //! \brief Checker for the sorted runs object created by the \c runs_creator .
    //! \param sruns sorted runs object
    //! \param cmp comparison object used for checking the order of elements in runs
    //! \return \c true if runs are sorted, \c false otherwise
    template <class RunsType_, class Cmp_>
    bool check_sorted_runs(const RunsType_ & sruns, Cmp_ cmp)
    {
        sort_helper::verify_sentinel_strict_weak_ordering(cmp);
        typedef typename RunsType_::block_type block_type;
        typedef typename block_type::value_type value_type;
        STXXL_VERBOSE2("Elements: " << sruns.elements);
        unsigned_type nruns = sruns.runs.size();
        STXXL_VERBOSE2("Runs: " << nruns);
        unsigned_type irun = 0;
        for (irun = 0; irun < nruns; ++irun)
        {
            const unsigned_type nblocks = sruns.runs[irun].size();
            block_type * blocks = new block_type[nblocks];
            request_ptr * reqs = new request_ptr[nblocks];
            for (unsigned_type j = 0; j < nblocks; ++j)
            {
                reqs[j] = blocks[j].read(sruns.runs[irun][j].bid);
            }
            wait_all(reqs, reqs + nblocks);
            for (unsigned_type j = 0; j < nblocks; ++j)
            {
                if (cmp(blocks[j][0], sruns.runs[irun][j].value) ||
                    cmp(sruns.runs[irun][j].value, blocks[j][0])) //!=
                {
                    STXXL_ERRMSG("check_sorted_runs  wrong trigger in the run");
                    return false;
                }
            }
            if (!stxxl::is_sorted(make_element_iterator(blocks, 0),
                                  make_element_iterator(blocks, sruns.runs_sizes[irun]),
                                  cmp))
            {
                STXXL_ERRMSG("check_sorted_runs  wrong order in the run");
                return false;
            }

            delete[] reqs;
            delete[] blocks;
        }

        STXXL_MSG("Checking runs finished successfully");

        return true;
    }


    ////////////////////////////////////////////////////////////////////////
    //     MERGE RUNS                                                     //
    ////////////////////////////////////////////////////////////////////////

    //! \brief Merges sorted runs
    //!
    //! \tparam RunsType_ type of the sorted runs, available as \c runs_creator::sorted_runs_type ,
    //! \tparam Cmp_ type of comparison object used for merging
    //! \tparam AllocStr_ allocation strategy used to allocate the blocks for
    //! storing intermediate results if several merge passes are required
    template <class RunsType_,
              class Cmp_,
              class AllocStr_ = STXXL_DEFAULT_ALLOC_STRATEGY>
    class basic_runs_merger : private noncopyable
    {
    protected:
        typedef RunsType_ sorted_runs_type;
        typedef AllocStr_ alloc_strategy;
        typedef typename sorted_runs_type::size_type size_type;
        typedef Cmp_ value_cmp;
        typedef typename sorted_runs_type::run_type run_type;
        typedef typename sorted_runs_type::block_type block_type;
        typedef block_type out_block_type;
        typedef typename run_type::value_type trigger_entry_type;
        typedef block_prefetcher<block_type, typename run_type::iterator> prefetcher_type;
        typedef run_cursor2<block_type, prefetcher_type> run_cursor_type;
        typedef sort_helper::run_cursor2_cmp<block_type, prefetcher_type, value_cmp> run_cursor2_cmp_type;
        typedef loser_tree<run_cursor_type, run_cursor2_cmp_type> loser_tree_type;
        typedef stxxl::int64 diff_type;
        typedef std::pair<typename block_type::iterator, typename block_type::iterator> sequence;
        typedef typename std::vector<sequence>::size_type seqs_size_type;

    public:
        //! \brief Standard stream typedef
        typedef typename sorted_runs_type::value_type value_type;

    private:
        sorted_runs_type sruns;
        value_cmp cmp;
        size_type elements_remaining;

        out_block_type * current_block;
        unsigned_type buffer_pos;
        value_type current_value;               // cache for the current value

        run_type consume_seq;
        int_type * prefetch_seq;
        prefetcher_type * prefetcher;
        loser_tree_type * losers;
#if STXXL_PARALLEL_MULTIWAY_MERGE
        std::vector<sequence> * seqs;
        std::vector<block_type *> * buffers;
        diff_type num_currently_mergeable;
#endif

#if STXXL_CHECK_ORDER_IN_SORTS
        value_type last_element;
#endif //STXXL_CHECK_ORDER_IN_SORTS
        
        ////////////////////////////////////////////////////////////////////

        void merge_recursively(unsigned_type memory_to_use);

        void deallocate_prefetcher()
        {
            if (prefetcher)
            {
                delete losers;
#if STXXL_PARALLEL_MULTIWAY_MERGE
                delete seqs;
                delete buffers;
#endif
                delete prefetcher;
                delete[] prefetch_seq;
                prefetcher = NULL;
            }
            // free blocks in runs , (or the user should do it?)
            sruns.deallocate_blocks();
        }

        void fill_current_block()
        {
            STXXL_VERBOSE1("fill_current_block");
            if (do_parallel_merge())
            {
#if STXXL_PARALLEL_MULTIWAY_MERGE
// begin of STL-style merging
                diff_type rest = out_block_type::size;          // elements still to merge for this output block

                do                                              // while rest > 0 and still elements available
                {
                    if (num_currently_mergeable < rest)
                    {
                        if (!prefetcher || prefetcher->empty())
                        {
                            // anything remaining is already in memory
                            num_currently_mergeable = elements_remaining;
                        }
                        else
                        {
                            num_currently_mergeable = sort_helper::count_elements_less_equal(
                                *seqs, consume_seq[prefetcher->pos()].value, cmp);
                        }
                    }

                    diff_type output_size = STXXL_MIN(num_currently_mergeable, rest);     // at most rest elements

                    STXXL_VERBOSE1("before merge " << output_size);

                    stxxl::parallel::multiway_merge((*seqs).begin(), (*seqs).end(), current_block->end() - rest, cmp, output_size);
                    // sequence iterators are progressed appropriately

                    rest -= output_size;
                    num_currently_mergeable -= output_size;

                    STXXL_VERBOSE1("after merge");

                    sort_helper::refill_or_remove_empty_sequences(*seqs, *buffers, *prefetcher);
                } while (rest > 0 && (*seqs).size() > 0);

#if STXXL_CHECK_ORDER_IN_SORTS
                if (!stxxl::is_sorted(current_block->begin(), current_block->end(), cmp))
                {
                    for (value_type * i = current_block->begin() + 1; i != current_block->end(); ++i)
                        if (cmp(*i, *(i - 1)))
                        {
                            STXXL_VERBOSE1("Error at position " << (i - current_block->begin()));
                        }
                    assert(false);
                }
#endif //STXXL_CHECK_ORDER_IN_SORTS

// end of STL-style merging
#else
                STXXL_THROW_UNREACHABLE();
#endif //STXXL_PARALLEL_MULTIWAY_MERGE
            }
            else
            {
// begin of native merging procedure
                losers->multi_merge(current_block->elem, current_block->elem + STXXL_MIN<size_type>(out_block_type::size, elements_remaining));
// end of native merging procedure
            }
            STXXL_VERBOSE1("current block filled");

            if (elements_remaining <= out_block_type::size)
                deallocate_prefetcher();
        }

    public:
        //! \brief Creates a runs merger object
        //! \param r input sorted runs object
        //! \param c comparison object
        //! \param memory_to_use amount of memory available for the merger in bytes
        basic_runs_merger(const sorted_runs_type & r, value_cmp c, unsigned_type memory_to_use) :
            sruns(r),
            cmp(c),
            elements_remaining(sruns.elements),
            current_block(NULL),
            buffer_pos(0),
            prefetch_seq(NULL),
            prefetcher(NULL),
            losers(NULL)
#if STXXL_PARALLEL_MULTIWAY_MERGE
            , seqs(NULL),
            buffers(NULL),
            num_currently_mergeable(0)
#endif
#if STXXL_CHECK_ORDER_IN_SORTS
            , last_element(cmp.min_value())
#endif //STXXL_CHECK_ORDER_IN_SORTS
        {
            initialize(r, memory_to_use);
        }

    protected:
        void initialize(const sorted_runs_type & r, unsigned_type memory_to_use)
        {
            sort_helper::verify_sentinel_strict_weak_ordering(cmp);

            sruns = r;
            elements_remaining = r.elements;

            if (empty())
                return;

            if (!sruns.small_run().empty())
            {
                // we have a small input <= B, that is kept in the main memory
                STXXL_VERBOSE1("basic_runs_merger: small input optimization, input length: " << elements_remaining);
                assert(elements_remaining == size_type(sruns.small_run().size()));
                assert(sruns.small_run().size() <= out_block_type::size);
                current_block = new out_block_type;
                std::copy(sruns.small_run().begin(), sruns.small_run().end(), current_block->begin());
                current_value = current_block->elem[0];
                buffer_pos = 1;

                return;
            }

#if STXXL_CHECK_ORDER_IN_SORTS
            assert(check_sorted_runs(r, cmp));
#endif //STXXL_CHECK_ORDER_IN_SORTS

            disk_queues::get_instance()->set_priority_op(request_queue::WRITE);

            int_type disks_number = config::get_instance()->disks_number();
            unsigned_type min_prefetch_buffers = 2 * disks_number;
            unsigned_type input_buffers = (memory_to_use > sizeof(out_block_type) ? memory_to_use - sizeof(out_block_type) : 0) / block_type::raw_size;
            unsigned_type nruns = sruns.runs.size();

            if (input_buffers < nruns + min_prefetch_buffers)
            {
                // can not merge runs in one pass
                // merge recursively:
                STXXL_WARNMSG_RECURSIVE_SORT("The implementation of sort requires more than one merge pass, therefore for a better");
                STXXL_WARNMSG_RECURSIVE_SORT("efficiency decrease block size of run storage (a parameter of the run_creator)");
                STXXL_WARNMSG_RECURSIVE_SORT("or increase the amount memory dedicated to the merger.");
                STXXL_WARNMSG_RECURSIVE_SORT("m=" << input_buffers << " nruns=" << nruns << " prefetch_blocks=" << min_prefetch_buffers);

                // check whether we have enough memory to merge recursively
                unsigned_type recursive_merge_buffers = memory_to_use / block_type::raw_size;
                if (recursive_merge_buffers < 2 * min_prefetch_buffers + 1 + 2) {
                    // recursive merge uses min_prefetch_buffers for input buffering and min_prefetch_buffers output buffering
                    // as well as 1 current output block and at least 2 input blocks
                    STXXL_ERRMSG("There are only m=" << recursive_merge_buffers << " blocks available for recursive merging, but "
                                 << min_prefetch_buffers << "+" << min_prefetch_buffers << "+1 are needed read-ahead/write-back/output, and");
                    STXXL_ERRMSG("the merger requires memory to store at least two input blocks internally. Aborting.");
                    throw bad_parameter("basic_runs_merger::sort(): INSUFFICIENT MEMORY provided, please increase parameter 'memory_to_use'");
                }

                merge_recursively(memory_to_use);

                nruns = sruns.runs.size();
            }

            assert(nruns + min_prefetch_buffers <= input_buffers);

            unsigned_type i;
            unsigned_type prefetch_seq_size = 0;
            for (i = 0; i < nruns; ++i)
            {
                prefetch_seq_size += sruns.runs[i].size();
            }

            consume_seq.resize(prefetch_seq_size);
            prefetch_seq = new int_type[prefetch_seq_size];

            typename run_type::iterator copy_start = consume_seq.begin();
            for (i = 0; i < nruns; ++i)
            {
                copy_start = std::copy(
                    sruns.runs[i].begin(),
                    sruns.runs[i].end(),
                    copy_start);
            }

            std::stable_sort(consume_seq.begin(), consume_seq.end(),
                             sort_helper::trigger_entry_cmp<trigger_entry_type, value_cmp>(cmp) _STXXL_SORT_TRIGGER_FORCE_SEQUENTIAL);

            const unsigned_type n_prefetch_buffers = STXXL_MAX(min_prefetch_buffers, input_buffers - nruns);

#if STXXL_SORT_OPTIMAL_PREFETCHING
            // heuristic
            const int_type n_opt_prefetch_buffers = min_prefetch_buffers + (3 * (n_prefetch_buffers - min_prefetch_buffers)) / 10;

            compute_prefetch_schedule(
                consume_seq,
                prefetch_seq,
                n_opt_prefetch_buffers,
                disks_number);
#else
            for (i = 0; i < prefetch_seq_size; ++i)
                prefetch_seq[i] = i;
#endif //STXXL_SORT_OPTIMAL_PREFETCHING

            prefetcher = new prefetcher_type(
                consume_seq.begin(),
                consume_seq.end(),
                prefetch_seq,
                STXXL_MIN(nruns + n_prefetch_buffers, prefetch_seq_size));

            if (do_parallel_merge())
            {
#if STXXL_PARALLEL_MULTIWAY_MERGE
// begin of STL-style merging
                seqs = new std::vector<sequence>(nruns);
                buffers = new std::vector<block_type *>(nruns);

                for (unsigned_type i = 0; i < nruns; ++i)                                       //initialize sequences
                {
                    (*buffers)[i] = prefetcher->pull_block();                                   //get first block of each run
                    (*seqs)[i] = std::make_pair((*buffers)[i]->begin(), (*buffers)[i]->end());  //this memory location stays the same, only the data is exchanged
                }
// end of STL-style merging
#else
                STXXL_THROW_UNREACHABLE();
#endif //STXXL_PARALLEL_MULTIWAY_MERGE
            }
            else
            {
// begin of native merging procedure
                losers = new loser_tree_type(prefetcher, nruns, run_cursor2_cmp_type(cmp));
// end of native merging procedure
            }

            current_block = new out_block_type;
            fill_current_block();

            current_value = current_block->elem[0];
            buffer_pos = 1;
        }

    public:
        //! \brief Standard stream method
        bool empty() const
        {
            return elements_remaining == 0;
        }

        //! \brief Standard stream method
        const value_type & operator * () const
        {
            assert(!empty());
            return current_value;
        }

        //! \brief Standard stream method
        const value_type * operator -> () const
        {
            return &(operator * ());
        }

        //! \brief Standard stream method
        basic_runs_merger & operator ++ ()  // preincrement operator
        {
            assert(!empty());

            --elements_remaining;

            if (buffer_pos != out_block_type::size)
            {
                current_value = current_block->elem[buffer_pos];
                ++buffer_pos;
            }
            else
            {
                if (!empty())
                {
                    fill_current_block();

#if STXXL_CHECK_ORDER_IN_SORTS
                    assert(stxxl::is_sorted(current_block->elem, current_block->elem + STXXL_MIN<size_type>(elements_remaining, current_block->size), cmp));
                    assert(!cmp(current_block->elem[0], current_value));
#endif //STXXL_CHECK_ORDER_IN_SORTS
                    current_value = current_block->elem[0];
                    buffer_pos = 1;
                }
            }

#if STXXL_CHECK_ORDER_IN_SORTS
            if (!empty())
            {
                assert(!cmp(current_value, last_element));
                last_element = current_value;
            }
#endif //STXXL_CHECK_ORDER_IN_SORTS

            return *this;
        }

        //! \brief Destructor
        //! \remark Deallocates blocks of the input sorted runs object
        virtual ~basic_runs_merger()
        {
            deallocate_prefetcher();
            delete current_block;
        }
    };


    template <class RunsType_, class Cmp_, class AllocStr_>
    void basic_runs_merger<RunsType_, Cmp_, AllocStr_>::merge_recursively(unsigned_type memory_to_use)
    {
        block_manager * bm = block_manager::get_instance();
        unsigned_type ndisks = config::get_instance()->disks_number();
        unsigned_type nwrite_buffers = 2 * ndisks;
        unsigned_type memory_for_write_buffers = nwrite_buffers * sizeof(block_type);

        // memory consumption of the recursive merger (uses block_type as out_block_type)
        unsigned_type recursive_merger_memory_prefetch_buffers = 2 * ndisks * sizeof(block_type);
        unsigned_type recursive_merger_memory_out_block = sizeof(block_type);
        unsigned_type memory_for_buffers = memory_for_write_buffers
                                           + recursive_merger_memory_prefetch_buffers
                                           + recursive_merger_memory_out_block;
        // maximum arity in the recursive merger
        unsigned_type max_arity = (memory_to_use > memory_for_buffers ? memory_to_use - memory_for_buffers : 0) / block_type::raw_size;

        unsigned_type nruns = sruns.runs.size();
        const unsigned_type merge_factor = optimal_merge_factor(nruns, max_arity);
        assert(merge_factor > 1);
        assert(merge_factor <= max_arity);
        while (nruns > max_arity)
        {
            unsigned_type new_nruns = div_ceil(nruns, merge_factor);
            STXXL_VERBOSE("Starting new merge phase: nruns: " << nruns <<
                          " opt_merge_factor: " << merge_factor << " max_arity: " << max_arity << " new_nruns: " << new_nruns);

            sorted_runs_type new_runs;
            new_runs.runs.resize(new_nruns);
            new_runs.runs_sizes.resize(new_nruns);
            new_runs.elements = sruns.elements;

            unsigned_type runs_left = nruns;
            unsigned_type cur_out_run = 0;
            unsigned_type elements_in_new_run = 0;

            while (runs_left > 0)
            {
                int_type runs2merge = STXXL_MIN(runs_left, merge_factor);
                elements_in_new_run = 0;
                for (unsigned_type i = nruns - runs_left; i < (nruns - runs_left + runs2merge); ++i)
                {
                    elements_in_new_run += sruns.runs_sizes[i];
                }
                const unsigned_type blocks_in_new_run1 = div_ceil(elements_in_new_run, block_type::size);

                new_runs.runs_sizes[cur_out_run] = elements_in_new_run;
                // allocate run
                new_runs.runs[cur_out_run++].resize(blocks_in_new_run1);
                runs_left -= runs2merge;
            }

            // allocate blocks for the new runs
            for (unsigned_type i = 0; i < new_runs.runs.size(); ++i)
                bm->new_blocks(alloc_strategy(), make_bid_iterator(new_runs.runs[i].begin()), make_bid_iterator(new_runs.runs[i].end()));

            // merge all
            runs_left = nruns;
            cur_out_run = 0;
            size_type elements_left = sruns.elements;

            while (runs_left > 0)
            {
                unsigned_type runs2merge = STXXL_MIN(runs_left, merge_factor);
                STXXL_VERBOSE("Merging " << runs2merge << " runs");

                sorted_runs_type cur_runs;
                cur_runs.runs.resize(runs2merge);
                cur_runs.runs_sizes.resize(runs2merge);

                std::copy(sruns.runs.begin() + nruns - runs_left,
                          sruns.runs.begin() + nruns - runs_left + runs2merge,
                          cur_runs.runs.begin());
                std::copy(sruns.runs_sizes.begin() + nruns - runs_left,
                          sruns.runs_sizes.begin() + nruns - runs_left + runs2merge,
                          cur_runs.runs_sizes.begin());

                runs_left -= runs2merge;
                /*
                   cur_runs.elements = (runs_left)?
                   (new_runs.runs[cur_out_run].size()*block_type::size):
                   (elements_left);
                 */
                cur_runs.elements = new_runs.runs_sizes[cur_out_run];
                elements_left -= cur_runs.elements;

                if (runs2merge > 1)
                {
                    basic_runs_merger<RunsType_, Cmp_, AllocStr_> merger(cur_runs, cmp, memory_to_use - memory_for_write_buffers);

                    {   // make sure everything is being destroyed in right time
                        buf_ostream<block_type, typename run_type::iterator> out(
                            new_runs.runs[cur_out_run].begin(),
                            nwrite_buffers);

                        size_type cnt = 0;
                        const size_type cnt_max = cur_runs.elements;

                        while (cnt != cnt_max)
                        {
                            *out = *merger;
                            if ((cnt % block_type::size) == 0) // have to write the trigger value
                                new_runs.runs[cur_out_run][cnt / size_type(block_type::size)].value = *merger;

                            ++cnt;
                            ++out;
                            ++merger;
                        }
                        assert(merger.empty());

                        while (cnt % block_type::size)
                        {
                            *out = cmp.max_value();
                            ++out;
                            ++cnt;
                        }
                    }
                }
                else
                {
                    bm->delete_blocks(
                        make_bid_iterator(new_runs.runs.back().begin()),
                        make_bid_iterator(new_runs.runs.back().end()));

                    assert(cur_runs.runs.size() == 1);

                    std::copy(cur_runs.runs.front().begin(),
                              cur_runs.runs.front().end(),
                              new_runs.runs.back().begin());
                    new_runs.runs_sizes.back() = cur_runs.runs_sizes.front();
                }

                ++cur_out_run;
            }
            assert(elements_left == 0);

            nruns = new_nruns;
            sruns = new_runs;
        }
    }


    //! \brief Merges sorted runs
    //!
    //! \tparam RunsType_ type of the sorted runs, available as \c runs_creator::sorted_runs_type ,
    //! \tparam Cmp_ type of comparison object used for merging
    //! \tparam AllocStr_ allocation strategy used to allocate the blocks for
    //! storing intermediate results if several merge passes are required
    template <class RunsType_,
              class Cmp_,
              class AllocStr_ = STXXL_DEFAULT_ALLOC_STRATEGY>
    class runs_merger : public basic_runs_merger<RunsType_, Cmp_, AllocStr_>
    {
    private:
        typedef RunsType_ sorted_runs_type;
        typedef basic_runs_merger<RunsType_, Cmp_, AllocStr_> base;
        typedef typename base::value_cmp value_cmp;
        typedef typename base::block_type block_type;

    public:
        //! \brief Creates a runs merger object
        //! \param r input sorted runs object
        //! \param c comparison object
        //! \param memory_to_use amount of memory available for the merger in bytes
        runs_merger(const sorted_runs_type & r, value_cmp c, unsigned_type memory_to_use) :
            base(r, c, memory_to_use)
        { }
    };


    ////////////////////////////////////////////////////////////////////////
    //     SORT                                                           //
    ////////////////////////////////////////////////////////////////////////

    //! \brief Produces sorted stream from input stream
    //!
    //! \tparam Input_ type of the input stream
    //! \tparam Cmp_ type of comparison object used for sorting the runs
    //! \tparam BlockSize_ size of blocks used to store the runs
    //! \tparam AllocStr_ functor that defines allocation strategy for the runs
    //! \remark Implemented as the composition of \c runs_creator and \c runs_merger .
    template <class Input_,
              class Cmp_,
              unsigned BlockSize_ = STXXL_DEFAULT_BLOCK_SIZE(typename Input_::value_type),
              class AllocStr_ = STXXL_DEFAULT_ALLOC_STRATEGY,
              class runs_creator_type = runs_creator<Input_, Cmp_, BlockSize_, AllocStr_> >
    class sort : public noncopyable
    {
        typedef typename runs_creator_type::sorted_runs_type sorted_runs_type;
        typedef runs_merger<sorted_runs_type, Cmp_, AllocStr_> runs_merger_type;

        runs_creator_type creator;
        runs_merger_type merger;

    public:
        //! \brief Standard stream typedef
        typedef typename Input_::value_type value_type;

        //! \brief Creates the object
        //! \param in input stream
        //! \param c comparator object
        //! \param memory_to_use memory amount that is allowed to used by the sorter in bytes
        sort(Input_ & in, Cmp_ c, unsigned_type memory_to_use) :
            creator(in, c, memory_to_use),
            merger(creator.result(), c, memory_to_use)
        {
            sort_helper::verify_sentinel_strict_weak_ordering(c);
        }

        //! \brief Creates the object
        //! \param in input stream
        //! \param c comparator object
        //! \param memory_to_use_rc memory amount that is allowed to used by the runs creator in bytes
        //! \param memory_to_use_m memory amount that is allowed to used by the merger in bytes
        sort(Input_ & in, Cmp_ c, unsigned_type memory_to_use_rc, unsigned_type memory_to_use_m) :
            creator(in, c, memory_to_use_rc),
            merger(creator.result(), c, memory_to_use_m)
        {
            sort_helper::verify_sentinel_strict_weak_ordering(c);
        }


        //! \brief Standard stream method
        bool empty() const
        {
            return merger.empty();
        }

        //! \brief Standard stream method
        const value_type & operator * () const
        {
            assert(!empty());
            return *merger;
        }

        const value_type * operator -> () const
        {
            assert(!empty());
            return merger.operator -> ();
        }

        //! \brief Standard stream method
        sort & operator ++ ()
        {
            ++merger;
            return *this;
        }
    };

    //! \brief Computes sorted runs type from value type and block size
    //!
    //! \tparam ValueType_ type of values ins sorted runs
    //! \tparam BlockSize_ size of blocks where sorted runs stored
    template <
        class ValueType_,
        unsigned BlockSize_>
    class compute_sorted_runs_type
    {
        typedef ValueType_ value_type;
        typedef BID<BlockSize_> bid_type;
        typedef sort_helper::trigger_entry<bid_type, value_type> trigger_entry_type;

    public:
        typedef sorted_runs<trigger_entry_type> result;
    };

//! \}
}

//! \addtogroup stlalgo
//! \{

//! \brief Sorts range of any random access iterators externally

//! \param begin iterator pointing to the first element of the range
//! \param end iterator pointing to the last+1 element of the range
//! \param cmp comparison object
//! \param MemSize memory to use for sorting (in bytes)
//! \param AS allocation strategy
//!
//! The \c BlockSize template parameter defines the block size to use (in bytes)
//! \warning Slower than External Iterator Sort
template <unsigned BlockSize,
          class RandomAccessIterator,
          class CmpType,
          class AllocStr>
void sort(RandomAccessIterator begin,
          RandomAccessIterator end,
          CmpType cmp,
          unsigned_type MemSize,
          AllocStr AS)
{
    STXXL_UNUSED(AS);
#ifdef BOOST_MSVC
    typedef typename streamify_traits<RandomAccessIterator>::stream_type InputType;
#else
    typedef __typeof__(stream::streamify(begin, end)) InputType;
#endif //BOOST_MSVC
    InputType Input(begin, end);
    typedef stream::sort<InputType, CmpType, BlockSize, AllocStr> sorter_type;
    sorter_type Sort(Input, cmp, MemSize);
    stream::materialize(Sort, begin);
}

//! \}

__STXXL_END_NAMESPACE

#endif // !STXXL_SORT_STREAM_HEADER
// vim: et:ts=4:sw=4
