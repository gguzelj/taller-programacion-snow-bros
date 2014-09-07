b2Fixture* Figura::GetFixtureList(){
	return body->GetFixtureList();
}

b2Vec2 Figura::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}
