#include "TMXXMLParser.h"
#include <sstream>
#include "base/FileUtils.h"

NS_OCF_BEGIN

TMXLayerInfo::TMXLayerInfo()
	: m_pTiles(nullptr)
	, m_layerSize(0, 0)
{
}

TMXLayerInfo::~TMXLayerInfo()
{
	if (m_pTiles) {
		free(m_pTiles);
		m_pTiles = nullptr;
	}
}

TMXTilesetInfo::TMXTilesetInfo()
	: m_firstGid(0)
	, m_tileSize(0, 0)
	, m_imageSize(0, 0)
{
}

TMXTilesetInfo::~TMXTilesetInfo()
{
}

Rect TMXTilesetInfo::getRectForGID(uint32_t gid)
{
	Rect rect;
	rect.m_size = m_tileSize;
	gid = gid - m_firstGid;

	int max = static_cast<int>(m_imageSize.x / m_tileSize.x);
	rect.m_position.x = (gid % max) * m_tileSize.x;
	rect.m_position.y = (gid / max) * m_tileSize.y;

	return rect;
}

TMXMapInfo* TMXMapInfo::create(const std::string& tmxFile)
{
	TMXMapInfo* tmxMapInfo = new TMXMapInfo();
	if (tmxMapInfo->initWithTMXFile(tmxFile)) {
		tmxMapInfo->autorelease();
		return tmxMapInfo;
	}
	OCF_SAFE_DELETE(tmxMapInfo);
	return nullptr;
}

TMXMapInfo::TMXMapInfo()
	: m_mapSize(0, 0)
	, m_tileSize(0, 0)
	, m_tilesetInfo(nullptr)
{
}

TMXMapInfo::~TMXMapInfo()
{
	OCF_SAFE_DELETE(m_tilesetInfo);
}

bool TMXMapInfo::initWithTMXFile(const std::string& tmxFile)
{
	return parseXMLFile(tmxFile);
}

bool TMXMapInfo::parseXMLFile(const std::string& xmlFile)
{
	auto filename = FileUtils::getInstance()->fullPathForFilename(xmlFile);

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
		return false;
	}

	auto mapElement = doc.FirstChildElement("map");
	m_mapSize.x = mapElement->IntAttribute("width");
	m_mapSize.y = mapElement->IntAttribute("height");
	m_tileSize.x = mapElement->IntAttribute("tilewidth");
	m_tileSize.y = mapElement->IntAttribute("tileheight");

	// タイルセットを解析
	m_tilesetInfo = new TMXTilesetInfo();
	auto tilesetElement = mapElement->FirstChildElement("tileset");
	m_tilesetInfo->m_name = tilesetElement->Attribute("name");
	m_tilesetInfo->m_firstGid = tilesetElement->IntAttribute("firstgid");
	m_tilesetInfo->m_tileSize.x = tilesetElement->FloatAttribute("tilewidth");
	m_tilesetInfo->m_tileSize.y = tilesetElement->FloatAttribute("tileheight");
	auto imageElement = tilesetElement->FirstChildElement("image");
	m_tilesetInfo->m_imageSource = imageElement->Attribute("source");
	m_tilesetInfo->m_imageSize.x = imageElement->FloatAttribute("width");
	m_tilesetInfo->m_imageSize.y = imageElement->FloatAttribute("height");

	// レイヤーを解析
	parseLayer(mapElement);

	return true;
}

void TMXMapInfo::parseLayer(tinyxml2::XMLElement* element)
{
	for (auto layerElement = element->FirstChildElement("layer");
		layerElement != nullptr;
		layerElement = layerElement->NextSiblingElement()) {
		TMXLayerInfo* layer = new TMXLayerInfo();

		layer->m_name = layerElement->Attribute("name");
		layer->m_layerSize.x = layerElement->IntAttribute("width");
		layer->m_layerSize.y = layerElement->IntAttribute("height");

		auto dataElement = layerElement->FirstChildElement("data");
		std::string encoding = dataElement->Attribute("encoding");
		std::string dataString = dataElement->GetText();

		uint32_t atribute = 0;
		if (encoding == "none") {
			atribute |= static_cast<uint32_t>(TMXLayerAttribute::None);
		}
		else if (encoding == "base64") {
			atribute |= static_cast<uint32_t>(TMXLayerAttribute::Base64);
		}
		else if (encoding == "gzip") {
			atribute |= static_cast<uint32_t>(TMXLayerAttribute::Gzip);
		}
		else if (encoding == "zlib") {
			atribute |= static_cast<uint32_t>(TMXLayerAttribute::Zlib);
		}
		else if (encoding == "csv") {
			atribute |= static_cast<uint32_t>(TMXLayerAttribute::CSV);
		}
		else {
			OCF_ASSERT(false);
		}

		if (atribute & static_cast<uint32_t>(TMXLayerAttribute::CSV)) {
			std::vector<std::string> gidTokens;
			std::stringstream filestr;
			filestr << dataString;
			std::string row;
			while (std::getline(filestr, row, '\n')) {
				std::string gid;
				std::istringstream rowstr(row);
				while (std::getline(rowstr, gid, ',')) {
					gidTokens.emplace_back(gid);
				}
			}

			int bufferSize = m_mapSize.x * m_mapSize.y;
			uint32_t* buffer = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * bufferSize));
			if (buffer == nullptr) {
				break;
			}

			uint32_t* bufferPtr = buffer;
			for (const auto& gidToken : gidTokens) {
				uint32_t tileGid = strtoul(gidToken.c_str(), nullptr, 10);
				*bufferPtr = tileGid;
				bufferPtr++;
			}

			layer->m_pTiles = buffer;
		}
		else {
			OCFLOG("OcfEngile: TMX data encoding format is currently not supported");
		}

		m_layers.emplace_back(layer);
	}
}

NS_OCF_END
