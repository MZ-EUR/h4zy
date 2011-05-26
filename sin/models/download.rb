class Download
  include DataMapper::Resource

  belongs_to :affiliate
  belongs_to :host

  property :id,         Serial
  property :url,        String
  property :host_id,    Integer
  property :affiliate_id, Integer
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
