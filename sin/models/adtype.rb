class Adtype
  include DataMapper::Resource

  has n, :campaigns

  property :id,         Serial
  property :title,      String
  property :cpv,        Boolean, :default => false
  property :active,     Boolean, :default => false
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
