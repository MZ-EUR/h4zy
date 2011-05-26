class Credit
  include DataMapper::Resource

  belongs_to :user
  belongs_to :affiliate

  property :id,           Serial
  property :user_id,      Integer
  property :affiliate_id, Integer
  property :amount,       Integer
  property :hosts,        Integer
  property :paid,         Boolean, :default => false
  property :paid_at,      DateTime
  property :created_at,   DateTime, :default => lambda { DateTime.now }
end
