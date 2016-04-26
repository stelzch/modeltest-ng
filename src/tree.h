/*
 * tree.h
 *
 *  Created on: Jun 9, 2015
 *      Author: diego
 */

#ifndef TREE_H_
#define TREE_H_

#include "global_defs.h"
#include "msa.h"
#include <cassert>
#include <string>

/* exceptions */
#define EXCEPTION_TREE_SCRIPT  101
#define EXCEPTION_TREE_MISSING 102
#define EXCEPTION_TREE_USER    103
#define EXCEPTION_TREE_MP      104

namespace modeltest
{

  class Tree
  {
  public:
    Tree (tree_type_t _type,
          std::string const& filename,
          mt_size_t _number_of_threads = 1,
          int _random_seed = 12345)
    : type(_type),
      tree_file(filename),
      n_tips(0),
      number_of_threads(_number_of_threads),
      random_seed(_random_seed)
    {
    }

    Tree (tree_type_t _type,
          Msa &_msa,
          mt_size_t _number_of_threads,
          int _random_seed)
        : type(_type),
          tree_file(""),
          n_tips(_msa.get_n_sequences()),
          number_of_threads(_number_of_threads),
          random_seed(_random_seed)
    {
        assert(type != tree_user_fixed);
        UNUSED(_msa);
    }

    virtual ~Tree ();

    /**
     * @brief get the number of tips
     * @return the number of tips
     */
    mt_size_t get_n_tips( void ) const { return n_tips; }

    /**
     * @brief get the number of inner nodes
     * @return the number of inner nodes
     */
    mt_size_t get_n_inner( void ) const { return n_inner; }

    /**
     * @brief get the number of branches
     * @return the number of branches
     */
    mt_size_t get_n_branches( void ) const { return 2*n_tips-3; }

    /**
     * @brief get the total number of nodes
     * @return the number of nodes
     */
    mt_size_t get_n_nodes( void ) const { return 2*n_tips-2; }

    virtual const std::string get_label( mt_index_t index, mt_index_t thread_number = 0 ) const = 0;

    /**
     * @brief place the root at a random node
     * @param thread_number the thread number
     */
    virtual void reroot_random(mt_index_t thread_number = 0) = 0;

    /**
     * @brief set all branches to a fixed length
     * @param length the branch length
     * @param thread_number the thread number
     * @return true, if OK
     */
    virtual bool set_branches(double length, mt_index_t thread_number = 0) = 0;

    /**
     * @brief scale all branches
     * @param factor the factor
     * @param thread_number the thread number
     * @return true, if OK
     */
    virtual bool scale_branches(double factor, mt_index_t thread_number = 0) = 0;

    /**
     * @brief reset all branches to the original lengths
     * @param thread_number the thread number
     * @return true, if OK
     */
    virtual bool reset_branches(mt_index_t thread_number = 0) = 0;

    virtual std::string newick( mt_index_t thread_number = 0 ) const = 0;

    /**
     * @brief extract a copy of the tree structure with branch lengths
     * @param thread_number the thread number
     * @return the tree structure
     */
    virtual void * extract_tree ( mt_index_t thread_number = 0 ) const = 0;

    /**
     * @brief check if branch lengths have been optimized
     * @return true, if branch lengths have been optimized
     */
    bool is_bl_optimized( void ) { return bl_optimized; }

    /**
     * @brief set the branch lengths as optimized
     */
    void set_bl_optimized( void ) { bl_optimized = true; }

  protected:
    tree_type_t type;               //! type of starting tree
    const std::string tree_file;  //! NEWICK tree filename
    mt_size_t n_tips;             //! number of tips
    mt_size_t n_inner;            //! number of inner nodes (2n_tips - 3)
    mt_size_t number_of_threads;  //! number of threads
    int random_seed;              //! RNG seed

    bool bl_optimized;            //! branch lengths are optimized
  };

} /* namespace modeltest */

#endif /* TREE_H_ */
