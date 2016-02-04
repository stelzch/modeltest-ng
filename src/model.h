#ifndef MODEL_H
#define MODEL_H

#include "model_defs.h"
#include "loggable.h"

#include <time.h>

#define PRINTMODEL_TABSIZE 20

namespace modeltest {

class Model : public Loggable
{
public:
    /**
     * @brief Creates a new Model
     * @param model_params model parameters mask
     */
    Model(mt_mask_t model_params);
    virtual ~Model();

    /**
     * @brief Clones all parameters from another model
     * @param other the other model
     */
    virtual void clone(const Model *other) = 0;

    virtual data_type_t get_datatype( void ) const = 0;

    mt_index_t get_matrix_index( void ) const;

    /**
     * @brief Gets the name of the model
     * @return the name of the model
     */
    std::string const& get_name( void ) const;

    /**
     * @brief Checks whether the model is already optimized
     * @return true, if the model is optimized
     */
    bool is_optimized( void ) const;

    /**
     * @brief Checks whether the model includes a proportion of invariant sites
     * @return true, if the model is +I
     */
    bool is_I( void ) const;

    /**
     * @brief Checks whether the model contains gamma rate heterogeneity
     * @return true, if the model is +G
     */
    bool is_G( void ) const;

    /**
     * @brief checks whether the model contains unequal frequencies
     * @return true, if state frequencies are unequal
     */
    bool is_F( void ) const;

    /**
     * @brief Gets the matrix symmetries
     * @return the rate matrix symmetries
     */
    virtual const int * get_symmetries( void ) const;

    /**
     * @brief Gets the number of states
     * @return the number of states
     */
    mt_size_t get_n_states( void ) const;

    /**
     * @brief Gets the number of substitution rates
     * @return the number of substitution rates
     */
    mt_size_t get_n_subst_rates( void ) const;

    /**
     * @brief Gets the model state frequencies
     * @return the state frequencies
     */
    const double * get_frequencies( void ) const;

    /**
     * @brief Sets the model state frequencies
     * @param[in] value the model state frequencies
     */
    void set_frequencies(const double value[]);
    void set_frequencies(const std::vector<double> & value);

    /**
     * @brief Gets the substitution rates
     * @return the substitution rates
     */
    virtual const double * get_subst_rates( void ) const;

    /**
     * @brief Sets the substitution rates
     * @param[in] value the new substitution rates
     * @param full_vector if false, contains only the rates
     *                    corresponding to non-equal rates in order
     */
    virtual void set_subst_rates(const double value[],
                         bool full_vector=true);

    /**
     * @brief Gets the number of model substitution rate parameters
     * @return the number of model substitution rate parameters
     */
    virtual mt_size_t get_n_subst_params( void ) const;

    /**
     * @brief Gets the number of model free parameters
     * @return the number of model free parameters
     */
    mt_size_t get_n_free_variables( void ) const;

    /**
     * @brief Gets the log-Likelihood score
     * @return the log-Likelihood score
     */
    double get_lnl( void ) const;

    /**
     * @brief Sets the log-Likelihood score
     * @param l the log-Likelihood score
     */
    void set_lnl( double l );

    /**
     * @brief Prints out the model
     * @param[in] out the output stream to print to
     */
    virtual void print(std::ostream  &out = std::cout) = 0;

    /**
     * @brief Prints out the model for logging
     * @param[in] out the output stream to print log to
     */
    virtual void output_log(std::ostream  &out) = 0;

    virtual void input_log(std::istream  &in) = 0;

    bool evaluate_criteria (mt_size_t n_branches_params,
                            double sample_size );

    double get_bic( void ) const;
    double get_aic( void ) const;
    double get_aicc( void ) const;
    double get_dt( void ) const;

    double get_prop_inv( void ) const;
    void set_prop_inv(double value);

    double get_alpha( void ) const;
    void set_alpha(double value);

    time_t get_exec_time() const;
    void set_exec_time( time_t t);

    pll_utree_t * get_tree( void ) const;
    void set_tree( pll_utree_t * tree );

protected:
    mt_index_t matrix_index;
    std::string name;

    bool optimize_pinv;
    bool optimize_gamma;
    bool optimize_freqs;

    double prop_inv;
    double alpha;
    double *frequencies;
    double *subst_rates;

    mt_size_t n_free_variables;

    double lnL;
    double bic;
    double aic;
    double aicc;
    double dt;

    time_t exec_time;

    mt_size_t n_frequencies;
    mt_size_t n_subst_rates;

    pll_utree_t *tree;
};

class DnaModel : public Model
{
public:
    DnaModel(mt_index_t matrix_index,
          int model_params);
    DnaModel(const Model &other);
    virtual void clone(const Model *other);

    virtual data_type_t get_datatype( void ) const
    {
        return dt_dna;
    }

    /**
     * @brief Gets the matrix symmetries
     * @return the rate matrix symmetries
     */
    virtual const int * get_symmetries( void ) const;
    virtual mt_size_t get_n_subst_params() const;
    virtual void set_subst_rates(const double value[],
                                 bool full_vector=true);
    virtual void print(std::ostream  &out = std::cout);
    virtual void output_log(std::ostream  &out);
    virtual void input_log(std::istream  &in);
private:
    int matrix_symmetries[N_DNA_SUBST_RATES];
};

class ProtModel : public Model
{
public:
    ProtModel(mt_index_t matrix_index,
          int model_params);
    ProtModel(const Model &other);
    virtual ~ProtModel( void );
    virtual void clone(const Model *other);

    virtual data_type_t get_datatype( void ) const
    {
        return dt_protein;
    }

    virtual mt_size_t get_n_subst_params( void ) const;
    virtual const double * get_subst_rates( void ) const;
    virtual void set_subst_rates(const double value[],
                                 bool full_vector=true);
    virtual void print(std::ostream  &out = std::cout);
    virtual void output_log(std::ostream  &out);
    virtual void input_log(std::istream  &in);

private:
    const double *fixed_subst_rates;
};

}
#endif // MODEL_H
