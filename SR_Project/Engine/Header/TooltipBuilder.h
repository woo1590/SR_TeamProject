#pragma once

BEGIN(Engine)

class ENGINE_DLL TooltipBuilder
{
public:
	static vector<TooltipLine> BuildTooltip(const ItemInfo& info)
	{
		vector<TooltipLine> lines;

		// 1. 아이템 이름
		lines.push_back({info.name, FontType::Title, Color::White});

		// 2. 주 효과 설명 (flavorText)
		if (!info.flavorText.empty())
			lines.push_back({info.flavorText, FontType::Large, Color::White});

		// 3. 세부 스탯 목록
		for (const auto& stat : info.stats)
		{
			wstring statText = stat.description + L": " + to_wstring((int)info.value) + stat.unit;
			lines.push_back({statText, FontType::Large, Color::Cyan, stat.iconKey});
		}

		// 4. 배경 이야기 (bgText)
		if (!info.bgText.empty())
			lines.push_back({info.bgText, FontType::QuestTitle, Color::Gray});

		return lines;
	}
};

END