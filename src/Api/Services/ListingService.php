<?php

namespace Etsy\Api\Services;

use Etsy\Api\Client;
use Etsy\Logger\Logger;

/**
 * Class ListingService
 */
class ListingService
{
	/**
	 * @var Client
	 */
	private $client;

	/**
	 * @var Logger
	 */
	private $logger;

	/**
	 * @param Client $client
	 * @param Logger $logger
	 */
	public function __construct(Client $client, Logger $logger)
	{
		$this->client = $client;
		$this->logger = $logger;
	}

	/**
	 * @param string $language
	 * @param array  $data
	 * @return int
	 */
	public function createListing($language, array $data)
	{
		$response = $this->client->call('createListing', ['language' => 'de'], $data);

		return (int) reset($response['results'])['listing_id']; // TODO maybe it's better to return the entire listing data?
	}

	/**
	 * @param int   $id
	 * @param array $data
	 * @return bool
	 */
	public function updateListing($id, $data)
	{
		$response = $this->client->call('updateListing', [
			'listing_id' => $id,
		], $data);

		return true;
	}
}
