<?php
class MySQLDB
{
	private $host;
	private $username;
	private $password;
	private $database;
	private $conn;

	public function __construct($host, $username, $password, $database)
	{
		$this->host = $host;
		$this->username = $username;
		$this->password = $password;
		$this->database = $database;
	}

	public function connect()
	{
		try {
			$this->conn = new PDO("mysql:host=$this->host;dbname=$this->database", $this->username, $this->password);
			$this->conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			return true;
		} catch (PDOException $e) {
			echo "Connection failed: " . $e->getMessage();
			return false;
		}
	}

	public function changeStatus($id, $newStatus)
	{
		try {
			$sql = "UPDATE solutions SET status=:newStatus WHERE id=:id";
			$stmt = $this->conn->prepare($sql);
			$stmt->bindParam(':newStatus', $newStatus);
			$stmt->bindParam(':id', $id);
			$stmt->execute();
			echo "Status updated successfully";
		} catch (PDOException $e) {
			echo "Error updating status: " . $e->getMessage();
		}
	}

	public function getStatus($id)
	{
		$stmt = $this->conn->prepare("SELECT status FROM solutions WHERE id = :id");
		$stmt->bindParam(':id', $id);
		$stmt->execute();
		$status = $stmt->fetchColumn();
		return $status;
	}


	public function close()
	{
		$this->conn = null;
	}
}


// $db = new MySQLDB("localhost", "username", "password", "mydb");
// $db->connect();
// $db->changeStatus(1, 3);
// $db->close();
