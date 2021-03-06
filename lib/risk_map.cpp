#include <algorithm>
#include <vector>

#include "risk_map.h"
#include "debug.h"

RiskMap::RiskMap(){}
RiskMap::~RiskMap(){
	this->clear();
}

const std::map<std::string, Continent>& RiskMap::getContinents() const {
	return this->continents;
}

const std::map<std::string, Country>& RiskMap::getCountries() const {
	return this->countries;
}
const std::map<std::string, Player>& RiskMap::getPlayers() const {
	return this->players;
}

/**
 * @brief Adds the provided continent to the map.
 * Countries must be added separately via addCountry().
 */
void RiskMap::addContinent(const Continent& continent){
	continents[continent.getName()] = continent;
	this->notifyObservers();
}

/**
 * @brief Gets a pointer to the continent given its name
 */
Continent* RiskMap::getContinent(const std::string& name){
	return &this->continents[name];
}

/**
 * @brief Gets a set of country names that belong to a continent
 */
string_set RiskMap::getCountriesInContinent(const std::string& continentName) {
	return mapGraph.subgraphContents(continentName);
}

/**
 * @brief Adds a country to the map belonging to the given continent.
 * The continent will be created if it does not yet exist.
 */
Country* RiskMap::addCountry(const Country& country, const std::string& continentName){
	if (continents.find(continentName) == continents.end()) {
		continents[continentName] = Continent(continentName);
	}
	if (countries.find(country.getName()) == countries.end()) {
		countries[country.getName()] = country;
        cards++;
	}
	if (!mapGraph.insertNode(country.getName(), continentName))
		return nullptr;

	this->notifyObservers();
	return &this->countries[country.getName()];
}

/**
 * @brief Gets a pointer to the country given its name
 */
Country* RiskMap::getCountry(const std::string& countryName){
	return &this->countries[countryName];
}

/**
 * @brief Removes a country from the map.
 */
void RiskMap::removeCountry(const Country& country){
	std::string continent = (this->getContinentOfCountry(country.getName()))->getName();
	countries.erase(country.getName());
	if (mapGraph.removeNode(country.getName())){
		continents.erase(continent);
	}
	this->notifyObservers();
}

/**
 * @brief Renames a country in the map
 */
void RiskMap::renameCountry(const std::string oldName, const std::string newName){
	auto it = countries.find(oldName);
	if (it != countries.end()){
		(it->second).setName(newName);
		std::swap(countries[newName], it->second);
		countries.erase(oldName);
		mapGraph.renameNode(oldName, newName);
		this->notifyObservers();
	}
}

/**
 * @brief Marks two countries as neighboring.
 * Makes countries attackable from one another.
 */
void RiskMap::addNeighbour(const std::string& country1, const std::string& country2){
	mapGraph.insertEdge(country1, country2);
	this->notifyObservers();
}

string_set RiskMap::getNeighbours(const std::string& countryName){
	return mapGraph.incidentEdges(countryName);
}

/**
 * @brief Verifies if two countries are adjacent/neighbors.
 */
bool RiskMap::areCountriesNeighbours(const std::string& country1, const std::string& country2) {
	return mapGraph.areAdjacent(country1, country2);
}

/**
 * @brief Unmarks two countries as neighboring.
 * Makes countries unattackable from one another.
 */
void RiskMap::removeNeighbour(const std::string& country1, const std::string& country2){
	mapGraph.removeEdge(country1, country2);
	this->notifyObservers();
}

/**
 * @brief Gets a pointer to the continent a country belongs to
 */
Continent* RiskMap::getContinentOfCountry(const std::string& countryName){
	std::string continentName(mapGraph.getSubgraphName(countryName));
	return &continents[continentName];
}

/**
 * @brief Adds a player to the map
 */
Player* RiskMap::addPlayer(const Player& player) {
	if (this->players.find(player.getName()) == this->players.end()) {
		this->players[player.getName()] = player;
	}
	this->notifyObservers();
	return &this->players[player.getName()];
}

/**
 * @brief Gets a player by their name
 */
Player* RiskMap::getPlayer(const std::string& playerName){
	return &this->players[playerName];
}

/**
 * @brief Gets a list of country names owned by a player
 */
string_set RiskMap::getCountriesOwnedByPlayer(const std::string& playerName)  {
	string_set countriesOwned;
	for (auto &ent1: this->countries) {
		Country& country = ent1.second;
		if (country.getPlayer() == playerName) {
			countriesOwned.insert(country.getName());
		}
	}
	return countriesOwned;
}

/**
 * @brief Gets a list of continent names owned by a player
 */
string_set RiskMap::getContinentsOwnedByPlayer(const std::string& playerName) {
	string_set continentsOwned;
	for (auto &ent1: this->continents) {
		std::string continentName = ent1.first;
		string_set countriesInContent = this->getCountriesInContinent(continentName);

		bool totalOwnership = true;
		for (auto &countryName: countriesInContent) {
			Country* country = this->getCountry(countryName);
			if (country->getPlayer() != playerName) {
				totalOwnership = false;
				break;
			}
		}

		if (totalOwnership) {
			continentsOwned.insert(continentName);
		}
	}
	return continentsOwned;
}

/**
 * @brief RiskMap::getCards returns the total number of cards available
 * @return
 */
int RiskMap::getCards()
{
    return cards;
}

/**
 * @brief RiskMap::takeCards Removes a number of cards from the deck
 * @param numberOfCards The number of cards to take from the available deck
 */
int RiskMap::updateCards(int numberOfCards)
{
    cards += numberOfCards;
    return cards;
}

/**
 * @brief Given path to a Conquest map file, build a new RiskMap object
 */
void RiskMap::load(const std::string& path) {
	struct stat buffer;
	bool exists = (stat (path.c_str(), &buffer) == 0);

	if (!exists) {
		return;
	}

	std::string extension = path.substr(path.size() - 3, 3);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	debug("Extension: " + extension);

	this->setNotificationsEnabled(false);
	if (extension == "map") {
		this->loadMap(path);
	}
	else if (extension == "xml") {
		this->loadXML(path);
	}
	this->setNotificationsEnabled(true);
}

/**
 * @brief Parses the Cereal XML map file in the path indicated to populate the instance
 */
void RiskMap::loadXML(const std::string& path) {
	std::ifstream infile(path);
	cereal::XMLInputArchive input(infile);
	input(cereal::make_nvp("maprisk", *this));
}

/**
 * @brief Parses the Conquest .map map file in the path indicated to populate the instance
 */
void RiskMap::loadMap(const std::string& path) {
	std::ifstream infile(path);
	std::string line;
	int mode = 0;

	while (std::getline(infile, line))
	{
		std::string debug_str("Read line: ");
		debug_str.append(line);
		debug(debug_str);
		// Windows prefers /r/n, but getline() breaks only on \n.
		if (line.size() > 0 && line[line.size() - 1] == '\r') {
			line.resize(line.size() - 1);
		}

		// Set the mode for how we should process lines based on section headers
		if (line.compare("[Map]") == 0) {
			mode = MAP_PARSE_MODE_MAP;
			debug("  Parsing map metadata");
			continue;
		}
		if (line.compare("[Continents]") == 0) {
			mode = MAP_PARSE_MODE_CONTINENTS;
			debug("  Parsing continents");
			continue;
		}
		if (line.compare("[Territories]") == 0) {
			mode = MAP_PARSE_MODE_COUNTRIES;
			debug("  Parsing countries");
			continue;
		}

		// Process lines per the current mode.
		std::string item;
		std::stringstream line_stream(line);
		std::vector<std::string> values;
		if (mode == MAP_PARSE_MODE_MAP || line.length() == 0) {
			debug_str = "  Skipping: ";
			debug_str.append(line);
			debug(debug_str);
			continue;
		}
		else if (mode == MAP_PARSE_MODE_CONTINENTS) {
			while (std::getline(line_stream, item, '=')) {
				values.push_back(item);
			}
			debug_str = "  Adding continent: ";
			debug_str.append(values[0]);
			debug(debug_str);

			Continent continent(values[0]);
			continent.setReinforcementBonus(atoi(values[1].c_str()));
			this->addContinent(continent);
		}
		else if (mode == MAP_PARSE_MODE_COUNTRIES) {
			while (std::getline(line_stream, item, ',')) {
				values.push_back(item);
			}
			std::string continentName(values[3]);

			debug_str = "  Adding country: ";
			debug_str.append(values[0]);
			debug_str.append(" in continent ");
			debug_str.append(continentName);
			debug(debug_str);

			Country country(values[0]);
			country.setPositionX(atoi(values[1].c_str()));
			country.setPositionY(atoi(values[2].c_str()));
			country.setArmies(0);
			this->addCountry(country, continentName);
		}
		else {
			debug("Error parsing line: " + line);
			return;
		}
	}

	debug("Parsing file again to configure adjacencies");
	infile.clear();
	infile.seekg(0, std::ios_base::beg);
	while (std::getline(infile, line))
	{
		std::string debug_str("Read line: ");
		debug_str.append(line);
		debug(debug_str);
		// Windows prefers /r/n, but getline() breaks only on \n.
		if (line.size() > 0 && line[line.size() - 1] == '\r') {
			line.resize(line.size() - 1);
		}

		// Set the mode for how we should process lines based on section headers
		if (line.compare("[Map]") == 0) {
			mode = MAP_PARSE_MODE_MAP;
			debug("  Parsing map metadata");
			continue;
		}
		if (line.compare("[Continents]") == 0) {
			mode = MAP_PARSE_MODE_CONTINENTS;
			debug("  Parsing continents");
			continue;
		}
		if (line.compare("[Territories]") == 0) {
			mode = MAP_PARSE_MODE_COUNTRIES;
			debug("  Parsing countries");
			continue;
		}

		// Process lines per the current mode.
		std::string item;
		std::stringstream line_stream(line);
		std::vector<std::string> values;
		if (mode != MAP_PARSE_MODE_COUNTRIES || line.length() == 0) {
			debug_str = "  Skipping: ";
			debug_str.append(line);
			debug(debug_str);
			continue;
		}
		else if (mode == MAP_PARSE_MODE_COUNTRIES) {
			while (std::getline(line_stream, item, ',')) {
				values.push_back(item);
			}
			Country* country = this->getCountry(values[0]);
			std::vector<std::string>::iterator iter;
			for (iter = values.begin() + 4; iter < values.end(); iter++) {
				Country* neighbour = this->getCountry(*iter);
				this->addNeighbour(country->getName(), neighbour->getName());

				debug_str = "  ";
				debug_str.append(country->getName());
				debug_str.append(" touches ");
				debug_str.append(neighbour->getName());
				debug(debug_str);
			}
		}
		else {
			debug("Error parsing line: " + line);
			return;
		}
	}

	debug("Finished parsing: " + path);
}

bool RiskMap::save(SaveType saveType, std::string path) {
	std::string extension = "";
	bool success = false;
	switch (saveType) {
		case MAP:
			extension = ".map";
			path.append(extension);
			success = this->saveMap(path);
			break;
		case XML:
			extension = ".xml";
			path.append(extension);
			success = this->saveXML(path);
			break;
		default:
			return false;
	}
	std::string debug_str("Saved map file to ");
	debug_str.append(path);
	debug(debug_str);

	return success;
}

/**
 * @brief Serialize the map data into a new Conquest map data file at the given path
 */
bool RiskMap::saveMap(const std::string& path) {
	std::ofstream outfile(path, std::ios::out);
	if (!outfile.is_open()) {
		return false;
	}

	outfile << "[Continents]" << std::endl;
	for (auto const &ent1 : this->continents) {
		const Continent& continent = ent1.second;
		outfile << continent.getName() << "=" << continent.getReinforcementBonus() << std::endl;
	}
	outfile << std::endl;

	outfile << "[Territories]" << std::endl;
	for (auto const &ent1 : this->countries) {
		const Country& country = ent1.second;
		const Continent* continent = this->getContinentOfCountry(country.getName());
		outfile << country.getName() << "," << country.getPositionX() << "," << country.getPositionY() << "," << continent->getName();
		for (auto const &neighbour_country : this->getNeighbours(country.getName())) {
			outfile << "," << neighbour_country;
		}
		outfile << std::endl;
	}
	outfile << std::endl;

	outfile.close();
	return true;
}

/**
 * @brief Erases all data about the map.
 */
bool RiskMap::saveXML(const std::string& path) {
	std::ofstream outfile(path, std::ios::out);
	if (!outfile.is_open()) {
		return false;
	}

	cereal::XMLOutputArchive archive( outfile );
	archive(cereal::make_nvp("maprisk",*this));

	return true;
}

void RiskMap::clear() {
	this->continents.clear();
	this->countries.clear();
	this->mapGraph = SubGraphADT();
	this->notifyObservers();
}

/**
 * @brief validate validates the map is valid.
 */
bool RiskMap::validate() {
	bool result;

	// Check all countries form a connected graph
	result = isConnectedGraph("");
	if (!result) {
		return false;
	}

	// Check each continent's countries are connected subgraphs
	for (auto const &ent1 : this->continents) {
		const Continent& continent = ent1.second;
		result = isConnectedGraph(continent.getName());
		if (!result) {
			return false;
		}
	}

	return true;
}

/**
 * @brief Helper method for isConnectedGraph().
 */
void RiskMap::isConnectedGraphHelper(std::map<const Country*, bool>& visited, Country* country, const std::string& limitTo) {
	if (limitTo.size() > 0 && this->getContinentOfCountry(country->getName())->getName().compare(limitTo) != 0) {
		return;
	}
	bool& was_visited = visited.at(country);
	if (was_visited) {
		return;
	}
	was_visited = true;

	for (auto const &neighbourName : this->getNeighbours(country->getName())) {
		Country* neighbour = this->getCountry(neighbourName);
		this->isConnectedGraphHelper(visited, neighbour, limitTo);
	}
}

/**
 * @brief Determines if the countries on the map are a connected graph.
 * @param limitTo Limits the search to the given continent (by string name)
 */
bool RiskMap::isConnectedGraph(const std::string& limitTo) {
	std::map<const Country*, bool> visited = std::map<const Country*, bool>();
	for (auto const &ent1 : this->countries) {
		const Country& country = ent1.second;

		if (limitTo.size() > 0 && this->getContinentOfCountry(country.getName())->getName().compare(limitTo) != 0) {
			continue;
		}

		visited.insert(std::pair<const Country*, bool>(&country, false));
	}

	Country* country = nullptr;
	if (limitTo.size() > 0) {
		country = this->getCountry(*this->getCountriesInContinent(limitTo).begin());
	}
	else {
		country = &this->countries.begin()->second;
	}
	this->isConnectedGraphHelper(visited, country, limitTo);

	for (auto const &ent1 : visited) {
		if (!ent1.second) {
			Country country = *ent1.first;
			debug("Country " + country.getName() + " is not connected.");
			return false;
		}
	}

	return true;
}
