#include "network.h"
#include "random.h"
#include <algorithm>
#include <string> 
#include <map>
#include <iostream>

using namespace std;
/*!
  This is a network of nodes with bidirectional links: if <b>(a, b)</b> is a link then <b>(b, a)</b> is also a link.

  Each node supports a *double* value, and the list of nodes is in fact defined by the list of their values.
 */




/*! 
    Resizes the list of nodes (\ref values) and also resets all values.
    After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
 */
void Network::resize(const size_t& new_size){
		values.clear();
		for (size_t i(0); i< new_size; i++){
			values.push_back(RNG.normal(0,1));
		}
	}    
    
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes of the two nodes 
  @param[out] success the link was succesfully inserted (true if both nodes exist and the link did not exist yet)
 */	
bool Network::add_link (const size_t& Nodea, const size_t& Nodeb){
	
		if (Nodea > values.size() or Nodea <= 0 or Nodeb > values.size() or Nodeb <= 0){
			return false; 
			}
		for (auto it : links){ //Parcours notre multimap
	if (it.first == Nodea and it.second == Nodeb){
		return false;
		}
	}
		links.insert(make_pair(Nodea,Nodeb));
		links.insert(make_pair(Nodeb,Nodea));
	return true;
}
		
    
    
/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) 
 * where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */
    size_t Network::random_connect(const double& mean_deg){
		links.clear();
		size_t counter(0);
		vector<size_t> othernodes;
		
		
		for (size_t i(0); i< values.size(); i++){
			othernodes.push_back (i);
		}
		
		for (size_t i(0); i < values.size(); i++){
			size_t NBofnodes (RNG.poisson(mean_deg));
			if (NBofnodes >= size()){ //Plafonne le nombre maximale de nodes
				NBofnodes = size() -1;
				}
			RNG.shuffle(othernodes); 
			//On va shuffle le tableau othernodes et
			//choisir les Nbofnode premières valeurs 
			//afin de ne jamais prendre deux vois le meme nodes
			size_t indexofnodes (0);
			for (size_t j(0); j < NBofnodes; j++){
				while ((indexofnodes < size()) and not (add_link (i , othernodes[indexofnodes]))){
					++indexofnodes;
					}
					if (indexofnodes < size()){
					++counter;
						}
					}
		}
	return counter;
}		  
		
    
    
    
/*! Resets node values with a vector of *n* new values.
  If the current size is *s* and *n<s* then only the first *n* values are changed.
  If *n>s* then only *s* values are used. 
  The internal \ref values vector keeps the same size.
  @param[in] vector of new node values,
  @param[out] number of nodes succesfully reset.
 */

    size_t Network::set_values(const std::vector<double>& NewNodesValues){
	size_t numbreset(0);
	if( size() == NewNodesValues.size()){
		values.clear();
		values = NewNodesValues;
		numbreset = size();
		} 
	else if(size() < NewNodesValues.size()){
	for (size_t i(0); i< size(); i++){
		values[i] = NewNodesValues[i];
		
		}
		numbreset = size();
	} else {
	for (size_t i(0); i< NewNodesValues.size(); i++){
		values[i] = NewNodesValues[i];
			}
			numbreset = NewNodesValues.size();	
		}
		return numbreset;
		
    }
    
/*! Number of nodes */
    size_t Network::size() const{
		return values.size();
	}
    
    
/*! Degree (number of links) of node no *n* */
    size_t Network::degree(const size_t &_node) const{
		size_t nb_of_node(0);
	for (auto it : links){ 
		if (it.first == _node){
			++nb_of_node;
		}
	}
	return nb_of_node;
}
    
    
/*! Value of node no *n* */
    double Network::value(const size_t &_n) const{
		return values[_n];
	}
/*! All node values in descending order */
    std::vector<double> Network::sorted_values() const{
		vector<double> sorted_values (values);
		sort(sorted_values.begin(), sorted_values.end());
		reverse(sorted_values.begin(), sorted_values.end());
		return sorted_values;
	}
    
    
    
/*! All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> Network::neighbors(const size_t& PrincipalNode) const{
		vector<size_t> neighbors;
		for(auto it : links){
			if (it.first == PrincipalNode){
				neighbors.push_back(it.second);
			}
		}
		return neighbors;
	}
				
