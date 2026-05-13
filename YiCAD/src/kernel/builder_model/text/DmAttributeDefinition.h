/*
 * Copyright (C) 2026 YiCAD Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file DmAttributeDefinition.h
/// @brief 块属性定义类

#ifndef DMATTRIBUTEDEFINITION
#define DMATTRIBUTEDEFINITION

#include "DmText.h"
#include "AttributeDefinitionData.h"

class DmAttributeDefinition : public DmText
{
	TYPESYSTEM_HEADER();
public:
	DmAttributeDefinition() = default;
	DmAttributeDefinition(DmEntity* parent, const TextData& textData, const AttributeDefinitionData& attrData);
	virtual ~DmAttributeDefinition() = default;

public:
	virtual DmEntity* clone() const override;
	virtual DM::EntityType getEntityType() const override;
	AttributeDefinitionData& getAttributeDataRef();
	AttributeDefinitionData getAttributeData() const;

	void update() override;

	QString getTag() const;
	void setTag(const QString& tag);

	QString getPrompt() const;
	void setPrompt(const QString& prompt);

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
	AttributeDefinitionData data;
};

#endif // !DMATTRIBUTEDEFINITION
