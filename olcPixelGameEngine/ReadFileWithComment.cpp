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
