#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<functional>
#include<fstream>
#include<stack>
#include<sstream>


namespace utils
{
	class datafile
	{
	public:
		// Set the STRING value a property(for a given index)
		inline void SetString(const std::string& sString, const size_t nItem = 0)
		{
			if (nItem >= m_vContent.size())
				m_vContent.resize(nItem + 1);
			m_vContent[nItem] = sString;
		}

		// Retrives the STRING value of a property
		inline const std::string GetString(const size_t nItem = 0) const
		{
			if (nItem > m_vContent.size())
				return "";
			else
				return m_vContent[nItem];
		}

		// Set the REAL value of a property
		inline void SetReal(const double d, const size_t nItem = 0)
		{
			SetString(std::to_string(d), nItem);
		}

		// Retrives the REAL value of a property
		inline const double GetReal(const size_t nItem = 0) const
		{
			return std::atof(GetString(nItem).c_str());
		}

		// Set the INT value of a property
		inline void SetInt(const int32_t d, const size_t nItem = 0)
		{
			SetString(std::to_string(d), nItem);
		}

		// Retrives the INT value of a property
		inline const int32_t GetInt(const size_t nItem = 0) const
		{
			return std::atoi(GetString(nItem).c_str());
		}

		// Return the number of values a property consists of.
		inline size_t GetValueCount() const
		{
			return m_vContent.size();
		}

		inline datafile& operator[](const std::string& name)
		{
			// Check if this node's map is already contain an object with this name?
			{
				// It did not, crate this object in the map. 
				// First get a vector id and link it with the name in the map.
				if (m_mapObjects.count(name) == 0)
				{
					m_mapObjects[name] = m_vecObjects.size();
					m_vecObjects.push_back({ name, datafile() });  // Create new object with this name and push to back of vecobject.
				}

				// If it exists, get its index from the  ap and return element of vecObejcts
				return m_vecObjects[m_mapObjects[name]].second;
			}
		}

		std::vector<std::string> GetContent()
		{
			return m_vContent;
		}

		// Read a datafile
		static bool Read(
			datafile& n,
			const std::string& sFileName,
			const char sListSep = ','
		)
		{
			// Open the file
			std::ifstream file(sFileName);
			if (file.is_open())
			{
				//These vars are outside of the readloop used to refer
				// to previouse iteration values in certain conditions.
				std::string sPropName = "";
				std::string sPropValue = "";

				// The top of stack is the current node. When we are done
				// with this node, pop out of the stack
				std::stack<std::reference_wrapper<datafile>> stkPath;
				stkPath.push(n);

				// Read the file line by line and parse it.
				while (!file.eof())
				{
					// Read line
					std::string line;
					std::getline(file, line);

					// Remove white space at begining and at ending of stirng
					auto trim = [](std::string& s)
						{
							s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
							s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
						};
					trim(line);
					if (!line.empty())
					{
						// Test if it's a comment
						if (line[0] == '#')
						{
							datafile comment;
							comment.m_bIsComment = true;
							// We can't use map to get index but it dosen't matter.
							stkPath.top().get().m_vecObjects.push_back({ line, comment });
						}
						else
						{
							//  If line has content, first check assignment
							size_t x = line.find_first_of("=");
							if (x != std::string::npos)
							{
								sPropName = line.substr(0, x);
								trim(sPropName);
								sPropValue = line.substr(x + 1, line.size());
								trim(sPropValue);

								// Check if value is in quote
								bool bInQuotes = false;
								std::string sToken;
								size_t nTokenCount = 0;
								for (auto const c : sPropValue)
								{
									// Check for quotatin marks
									if (c == '\"')
									{
										bInQuotes = !bInQuotes; // Changes state to start write into " "
									}
									else
									{
										if (bInQuotes)
										{
											sToken.append(1, c); // Append into a " " until state changes
											//trim(sToken);
										}
										else
										{
											// Is the char is separato(r?
											if (c == sListSep)
											{
												// Set token value to file and up the token
												trim(sToken);
												stkPath.top().get()[sPropName].SetString(sToken, nTokenCount);
												sToken.clear();
												nTokenCount++;
											}
											else
											{
												// Just append.
												sToken.append(1, c);
												//trim(sToken);
											}
										}
									}
								}
								// The last token of line
								if (!sToken.empty())
								{
									trim(sToken);
									stkPath.top().get()[sPropName].SetString(sToken, nTokenCount);
								}
							}
							// No assignment
							else
							{
								// It's not valid syntax unless is '{'
								if (line[0] == '{')
								{
									// The propname is new node.
									stkPath.push(stkPath.top().get()[sPropName]);

								}
								else
								{
									if (line[0] == '}')
										stkPath.pop();
									else
									{
										// or its just a name of node.
										sPropName = line;
									}
								}
							}
						}

					}
					else
					{
						// Not read empty line. 
					}
				}

				file.close();
				return true;
			}
			else return false;
		}


		// Write a "datafile" node(and all of its child nodes and properties to a file
		inline static bool Write(
			const datafile& n,
			const std::string& sFileName,
			const std::string& sIndent = "\t",
			const char sListSep = ','
		)
		{

			std::string sSeparator = std::string(1, sListSep) + " ";
			// Level of indentation
			size_t nIndentCount = 0;

			// Recursive lambda function take reference of datafile node and file stream.
			std::function<void(const datafile&, std::ofstream&)> write = [&](const datafile& n, std::ofstream& file)
				{
					// Utitlity to write string in the correct indent. 
					auto indent = [&](const std::string& sString, const size_t indentCount)
						{
							std::string sOut;
							for (size_t i = 0; i < indentCount; i++) sOut += sString;
							return sOut;
						};

					// Writing out the node children 
					for (auto const& property : n.m_vecObjects)
					{
						// Check whether it contains any subobjects
						if (property.second.m_vecObjects.empty())
						{
							file << indent(sIndent, nIndentCount) << property.first << (property.second.m_bIsComment ? " " : "=");
							size_t nItems = property.second.GetValueCount();
							for (size_t i = 0; i < nItems; i++)
							{
								// If the value in string form contains the separation character, the value must be written inside quotation marks
								// If the value is the last of a list, it is not suffixesd with the separator. 
								size_t x = property.second.GetString(i).find_first_of(sListSep);
								if (x != std::string::npos)
								{
									file << "\"" << property.second.GetString(i) << "\"" << ((i < nItems - 1) ? sSeparator : "");
								}
								else
								{
									file << property.second.GetString(i) << ((i < nItems - 1) ? sSeparator : "");
								}
							}
							// Property written, move to the next line
							file << "\n";
						}
						else
						{
							// It has propeties of its own, so it's a node. 
							// Force a new line and write the out the node's name
							file << "\n" << indent(sIndent, nIndentCount) << property.first << "\n";
							// Open braces, and update indentation
							file << indent(sIndent, nIndentCount) << "{\n";
							nIndentCount++;

							// Recursively write that node
							write(property.second, file);

							// Node written, close the braces
							file << indent(sIndent, --nIndentCount) << "}\n\n";
						}

					}
				};

			// Start here. Open the file for writing
			//std::ofstream file(sFileName);
			std::ofstream file(sFileName);
			if (file.is_open())
			{
				write(n, file);
				return true;
			}
			else
			{
				std::cout << "error";
				return false;
			}
		}

		// Checks if a property exist
		inline bool HasProperty(const std::string& sName) const
		{
			return m_mapObjects.count(sName) > 0;
		}

		// Access a datafile via a convenient name- "read.node.something.property"
		inline datafile& GetProperty(const std::string& name)
		{
			size_t x = name.find_first_of('.');
			if (x != std::string::npos)
			{
				std::string sProperty = name.substr(0, x);
				if (HasProperty(sProperty))
				{
					return operator[](sProperty).GetProperty(name.substr(x + 1, name.size()));
				}
				else
					return operator[](sProperty);
			}
			else
				return operator[](name);
		}

		inline datafile& GetIndexProperty(const std::string& name, const size_t index)
		{
			return GetProperty(name + "[" + std::to_string(index) + "]");
		}

	private:
		// The list of string
		std::vector<std::string> m_vContent;
		std::vector < std::pair<std::string, utils::datafile>> m_vecObjects;
		std::unordered_map<std::string, size_t> m_mapObjects;

	protected:
		bool m_bIsComment = false;

	};
};

//
//int main()
//{
//	utils::datafile df;
//	/*df.SetReal(3.14);
//	std::cout << df.GetReal(0) << std::endl;*/
//
//	df["some_node"]["name"].SetString("tien");
//	df["some_node"]["age"].SetInt(27);
//	df["some_node"]["heigh"].SetReal(1.76);
//
//	//std::cout << df.GetContent()[0] << "-" << df.GetContent()[1] << "-" << df.GetContent()[2] << std::endl;
//	auto& a = df["some_node"];
//	a["name"].SetString("Tien");
//	a["age"].SetInt(27);
//	a["height"].SetReal(1.77);
//
//	auto& b = a["code"];
//	b.SetString("C++", 0);
//	b.SetString("vhdl,", 1);
//	b.SetString("lua", 2);
//	b.SetString("depzai,", 3);
//
//	auto& c = a["pc"];
//	c["processor"].SetString("intel");
//	c["ram"].SetInt(32);
//
//	a["SimilarThing"].SetInt(10);
//	for (size_t i = 0; i < 10; i++)
//		a["SimilarThing[" + std::to_string(i) + "]"].SetInt(rand());
//
//	//utils::datafile::Write(df, "TestOutput.txt");
//	utils::datafile test1;
//	utils::datafile::Read(test1, "TestOutput.txt");
//
//	utils::datafile::Write(test1, "TextOutput2.txt");
//	return 0;
//}