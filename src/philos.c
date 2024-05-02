/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 22:00:47 by akernot           #+#    #+#             */
/*   Updated: 2023/08/25 16:27:01 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdio.h>
#include <unistd.h>

#include <assert.h>

int64_t	timestamp(t_timeval *time_0, t_timeval *time)
{
	int64_t	timestamp;

	if (time_0 == NULL)
		return (0);
	timestamp = (time->tv_sec - time_0->tv_sec) * 1000;
	timestamp += (time->tv_usec - time_0->tv_usec) / 1000;
	return (timestamp);
}

void	change_action(t_program_data *data,
	t_philo *philo, t_timeval *current_time)
{
	if (philo->state == thinking && take_forks(philo) == true)
	{
		philo->last_ate = timestamp(&(data->time_0), current_time);
		philo->time_end_action
			= timestamp(&(data->time_0), current_time)
			+ data->time_to_eat;
		philo->state = eating;
		announce(data, philo->philo_id, philo->state);
	}
	else if (philo->state == thinking)
		return ;
	else if (philo->state == eating)
	{
		philo->time_end_action = timestamp(&(data->time_0), current_time)
			+ data->time_to_sleep;
		philo->state = sleeping;
		announce(data, philo->philo_id, philo->state);
		return_forks(philo);
	}
	else if (philo->state == sleeping)
	{
		philo->state = thinking;
		announce(data, philo->philo_id, philo->state);
		usleep(500);
	}
}

static void	end(t_program_data *data, t_philo *philo, t_state state)
{
	pthread_mutex_lock(&data->announcing);
	if (data->philo_died == true)
	{
		pthread_mutex_unlock(&data->announcing);
		return ;
	}
	if (state == dead)
	{
		philo->state = dead;
		pthread_mutex_unlock(&data->announcing);
		announce(data, philo->philo_id, dead);
		return ;
	}
	++(data->philos_fin);
	pthread_mutex_unlock(&data->announcing);
}

void	live(t_program_data *data, t_philo *philo)
{
	int64_t		current_time;
	t_timeval	time;
	int			i;

	i = 0;
	while (data->philo_died == false && data->philos_fin < data->num_philos)
	{
		gettimeofday(&time, NULL);
		current_time = timestamp(&(data->time_0), &time);
		if (current_time > philo->last_ate + data->time_to_die)
		{
			end(data, philo, dead);
			return ;
		}
		if (current_time < philo->time_end_action)
		{
			usleep(500);
			continue ;
		}
		change_action(data, philo, &time);
		if (philo->state == sleeping && i == data->num_times_philo_eat)
			end(data, philo, finished);
		if (philo->state == eating)
			++i;
	}
}

void	*init_philo(void *philo_packet)
{
	t_packet		*packet;
	t_program_data	*data;
	t_philo			new_philo;
	struct timeval	time;

	packet = (t_packet *)philo_packet;
	data = packet->data;
	new_philo.left_fork = packet->left_fork;
	new_philo.right_fork = packet->right_fork;
	new_philo.philo_id = packet->philo_id;
	new_philo.state = thinking;
	gettimeofday(&time, NULL);
	new_philo.last_ate = timestamp(&(data->time_0), &time);
	new_philo.time_end_action = 0;
	announce(data, packet->philo_id, thinking);
	live(data, &new_philo);
	return (NULL);
}
